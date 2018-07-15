#pragma once

#include <iostream>

#include <nlohmann/json.hpp>

#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/matrix.hpp>

namespace detail
{

template <glm::length_t L, typename T, glm::qualifier Q, size_t... Ns>
std::ostream& OstreamSerialize(std::ostream& out, const glm::vec<L, T, Q>& vec, std::index_sequence<Ns...>)
{
    out << "{ " << vec[0]; ((out << ", " << vec[Ns + 1]), ...); out << " }";

    return out;
}

}

/// Serializes GLM vector classes to output stream
template <glm::length_t L, typename T, glm::qualifier Q>
std::ostream& operator << (std::ostream& out, const glm::vec<L, T, Q>& vec)
{
    return detail::OstreamSerialize(out, vec, std::make_index_sequence<L - 1>{});
}

namespace detail
{

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q, size_t... Ns>
std::ostream& OstreamSerialize(std::ostream& out, const glm::mat<C, R, T, Q>& mat, std::index_sequence<Ns...>)
{
    const glm::mat<C, R, T, Q> tra = glm::transpose(mat);
    out << "{ \n" << tra[0]; 
    ((out << ", \n" << tra[Ns + 1]), ...); 
    out << "\n }";

    return out;
}

}

/// Serializes GLM matrix classes to output stream
template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
std::ostream& operator << (std::ostream& out, const glm::mat<C, R, T, Q>& mat)
{
    return detail::OstreamSerialize(out, mat, std::make_index_sequence<C - 1>{});
}

namespace nlohmann
{

///
template <glm::length_t L, typename T, glm::qualifier Q>
struct adl_serializer<glm::vec<L, T, Q>>
{
    ///
    template <size_t... Ns>
    static void Serialize(nlohmann::json& j, const glm::vec<L, T, Q>& value, std::index_sequence<Ns...>)
    {
        ((j[Ns] = value[Ns]), ...);
    }

    ///
    static void to_json(nlohmann::json& j, const glm::vec<L, T, Q>& value)
    {
        Serialize(j, value, std::make_index_sequence<L>{});
    }

    ///
    template <size_t... Ns>
    static void Deserialize(const nlohmann::json& j, glm::vec<L, T, Q>& value, std::index_sequence<Ns...>)
    {
        ((value[Ns] = j[Ns]), ...);
    }

    ///
    static void from_json(const nlohmann::json& j, glm::vec<L, T, Q>& value)
    {
        Deserialize(j, value, std::make_index_sequence<L>{});
    }

};

///
template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct adl_serializer<glm::mat<C, R, T, Q>>
{
    ///
    template <size_t... Ns>
    static void Serialize(nlohmann::json& j, const glm::mat<C, R, T, Q>& value, std::index_sequence<Ns...>)
    {
        ((j[Ns] = value[Ns]), ...);
    }

    ///
    static void to_json(nlohmann::json& j, const glm::mat<C, R, T, Q>& value)
    {
        Serialize(j, value, std::make_index_sequence<C>{});
    }

    ///
    template <size_t... Ns>
    static void Deserialize(const nlohmann::json& j, glm::mat<C, R, T, Q>& value, std::index_sequence<Ns...>)
    {
        ((value[Ns] = j[Ns]), ...);
    }

    ///
    static void from_json(const nlohmann::json& j, glm::mat<C, R, T, Q>& value)
    {
        Deserialize(j, value, std::make_index_sequence<C>{});
    }

};

}

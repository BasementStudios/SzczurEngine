#pragma once

/** @file SceneObject.hpp
 ** @description Header file with standard object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include <SFML/System/Vector2.hpp>

#include "Szczur/Modules/Assets/Assets.hpp" // @todo ! RREEEEEE
#include "Szczur/Modules/World/Displayer.hpp"

namespace rat
{

class SceneObject // @todo , sfDrawable?
{
    /* Types */
public:
    using Vector_t = sf::Vector2f;
    using Displayer_t = rat::Displayer<SceneObject>;
    


    /* Variables */
protected:
    std::string     _type       {""};
    Vector_t        _position   {0.f, 0.f};
    Vector_t        _speed      {0.f, 0.f};
    std::string     _state      {"default"};
    Displayer_t*    _displayer  = nullptr;

    Assets* _assetsStupidPtr = nullptr; // @todo ! RREEEEEEEEEE ;f
    


    /* Properties */
public:
    /// Type
    const std::string& getType() const; // @todo , nie powinien być tu/i nizej/ stringview? :thinking:
    void setType(const std::string& type);
    
    /// State
    const std::string& getState() const;
    void               setState(const std::string& state);
    
    /// Position
    Vector_t getPosition() const;
    void     setPosition(const Vector_t& position);
    
    /// Speed
    Vector_t getSpeed() const;
    void     setSpeed(const Vector_t& speed);



    /* Operators */
public:
    /// Constructors/destructor
    SceneObject();
    SceneObject(Assets* assetsStupidPtr, const std::string& type, const std::string& state = "default");
    
    ~SceneObject();



    /* Methods */
public:
    /// update
    void update(float deltaTime);

    /// draw
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

}
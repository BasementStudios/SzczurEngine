#include "Windows1250.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"

namespace rat
{

std::string mapWindows1250ToUtf8(const std::string& src)
{
    if (src.empty())
    {
        return {};
    }

    std::string out; out.reserve(src.size() * 2);

    for (size_t v = 0u; v < src.size(); ++v)
    {
        if (src[v] < 0)
        {
            switch (static_cast<unsigned>(src[v]))
            {
                case 0xA5: out += 0xC4; out += 0x84; break; // Ą
                case 0xB9: out += 0xC4; out += 0x85; break; // ą
                case 0xC6: out += 0xC4; out += 0x86; break; // Ć
                case 0xE6: out += 0xC4; out += 0x87; break; // ć
                case 0xCA: out += 0xC4; out += 0x98; break; // Ę
                case 0xEA: out += 0xC4; out += 0x99; break; // ę
                case 0xA3: out += 0xC5; out += 0x81; break; // Ł
                case 0xB3: out += 0xC5; out += 0x82; break; // ł
                case 0xD1: out += 0xC5; out += 0x83; break; // Ń
                case 0xF1: out += 0xC5; out += 0x84; break; // ń
                case 0xD3: out += 0xC3; out += 0x93; break; // Ó
                case 0xF3: out += 0xC3; out += 0xB3; break; // ó
                case 0x8C: out += 0xC5; out += 0x9A; break; // Ś
                case 0x9C: out += 0xC5; out += 0x9B; break; // ś
                case 0x8F: out += 0xC5; out += 0xB9; break; // Ź
                case 0x9F: out += 0xC5; out += 0xBA; break; // ź
                case 0xAF: out += 0xC5; out += 0xBB; break; // Ż
                case 0xBF: out += 0xC5; out += 0xBC; break; // ż
                default: break;
            }
        }
        else
        {
            out += src[v];
        }
    }

    return out;
}

std::string mapUtf8ToWindows1250(const std::string& src)
{
    if (src.empty())
    {
        return {};
    }

    std::string out; out.reserve(src.size());

    for (size_t v = 0u; v < src.size(); ++v)
    {
        if (src[v] < 0)
        {
            switch (static_cast<unsigned>(src[v]))
            {
                case 0xC3: switch (static_cast<unsigned>(src[v + 1u]))
                {
                    case 0x93: out += 0xD3; break; // Ó
                    case 0xB3: out += 0xF3; break; // ó
                }
                break;

                case 0xC4: switch (static_cast<unsigned>(src[v + 1u]))
                {
                    case 0x84: out += 0xA5; break; // Ą
                    case 0x85: out += 0xB9; break; // ą
                    case 0x86: out += 0xC6; break; // Ć
                    case 0x87: out += 0xE6; break; // ć
                    case 0x98: out += 0xCA; break; // Ę
                    case 0x99: out += 0xEA; break; // ę
                }
                break;

                case 0xC5: switch (static_cast<unsigned>(src[v + 1u]))
                {
                    case 0x81: out += 0xA3; break; // Ł
                    case 0x82: out += 0xB3; break; // ł
                    case 0x83: out += 0xD1; break; // Ń
                    case 0x84: out += 0xF1; break; // ń
                    case 0x9A: out += 0x8C; break; // Ś
                    case 0x9B: out += 0x9C; break; // ś
                    case 0xB9: out += 0x8F; break; // Ź
                    case 0xBA: out += 0x9F; break; // ź
                    case 0xBB: out += 0xAF; break; // Ż
                    case 0xBC: out += 0xBF; break; // ż
                }
                break;

                default: break;
            }

            ++v;
        }
        else
        {
            out += src[v];
        }
    }

    return out;
}

}

#pragma GCC diagnostic pop

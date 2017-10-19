#pragma once

#include <iterator>

namespace rat {
    template <typename InputIt, typename Key>
    constexpr InputIt interpolationSearch(InputIt begin, InputIt end, Key key) {
        InputIt low = begin;
        InputIt high = std::prev(end);
        InputIt middle;

        while(low != high && key >= *low && key <= *high) {
            middle = std::next(low, (key - *low) * std::distance(low, high) / (*high - *low));

            if(*middle < key)
                low = std::next(middle);
            else if(*middle > key)
                high = std::prev(middle);
            else
                return middle;
        }

        if(*begin == key)
            return begin;
        else
            return end;
    }

    template <typename InputIt, typename Key, typename Extractor>
    constexpr InputIt interpolationSearch(InputIt begin, InputIt end, Key key, Extractor extr) {
        InputIt low = begin;
        InputIt high = std::prev(end);
        InputIt middle;

        while(low != high && key >= extr(*low) && key <= extr(*high)) {
            middle = std::next(low, (key - extr(*low)) * std::distance(low, high) / (extr(*high) - extr(*low)));

            if(extr(*middle) < key)
                low = std::next(middle);
            else if(extr(*middle) > key)
                high = std::prev(middle);
            else
                return middle;
        }

        if(extr(*begin) == key)
            return begin;
        else
            return end;
    }
}

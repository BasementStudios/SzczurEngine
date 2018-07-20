#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

namespace sf { class Texture; }

namespace rat
{
    class GrayPPBar : public BaseBar
    {
    public:
        GrayPPBar();
        bool isEmpty() const;
        void fill();
        void empty();
        void lock();
        void unlock();

        void setSlotTextures(sf::Texture* tex);

        void dim();
        void undim();

    private:
        bool _isDimmed{false};
        enum class State { Empty, Full, Locked } _state = State::Full;
        ImageWidget* _slot{nullptr};

        void _updateTextRect();

        static constexpr int ppDim = 90;

    };
}
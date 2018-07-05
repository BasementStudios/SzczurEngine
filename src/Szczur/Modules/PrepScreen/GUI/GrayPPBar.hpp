#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

namespace sf { class Texture; }

namespace rat
{
    class GrayPPBar : public BaseBar
    {
    public:
        GrayPPBar();
        bool isTaken() const;
        void take();
        void returnTo();

        void setSlotTextures(sf::Texture* tex);

        void dim();
        void undim();

    private:
        bool _isTaken{true};
        enum class State { Empty, Full, Dimmed } _state = State::Full;
        ImageWidget* _slot{nullptr};

        void _updateTextRect();

        static constexpr int ppDim = 288;

    };
}
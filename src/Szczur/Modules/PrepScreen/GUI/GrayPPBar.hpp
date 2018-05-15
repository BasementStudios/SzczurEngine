#include "BaseBar.hpp"

namespace rat
{
    class GrayPPBar : public BaseBar
    {
    public:
        GrayPPBar();
        bool isTaken() const;
        void take();
        void returnTo();

        void setContainerTexture(sf::Texture* tex);
        void setPPTexture(sf::Texture* tex);

    private:
        ImageWidget* _container{nullptr};
        ImageWidget* _pp{nullptr};

        constexpr static float _border = 67.f;

    };
}
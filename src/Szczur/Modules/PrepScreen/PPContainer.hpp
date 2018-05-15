#pragma once

namespace rat
{
    class PPContainer
    {
        using amount_t = unsigned long long int;
        using power_t = unsigned long long int;
        
    public:
        PPContainer();

        amount_t getTotalAmount() const;
        amount_t getAmount() const;

        void add(amount_t addon);
        void remove(amount_t sub);

        void takeFrom(amount_t takenAmount);
        void returnTo(amount_t returnAmount);

        void reset();
    private:
        amount_t _actualAmount;
        amount_t _totalAmount;
    };
}
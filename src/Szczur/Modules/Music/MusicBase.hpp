#pragma once

#include <string>
#include <boost/container/flat_map.hpp>

#include "Szczur/Modules/Script/Script.hpp"

#include "RatMusic.hpp"

namespace rat 
{
	class MusicBase
	{

		enum class CallbackType 
        {
            onStart,
            onFinish,
			onFadeStart
        };

	public:

		using SolFunction_t = sol::function;
        using CallbacksContainer_t = boost::container::flat_map<CallbackType, SolFunction_t>;

	private:

		RatMusic* _base;
		
		float _timeLeft;
		float _baseVolume;

		bool _isEnding = false;
		bool _finishing = false;

		bool _finishInit = true;
		bool _startInit = true;

		CallbacksContainer_t _callbacks;

	public:

		MusicBase(RatMusic* source);

		static void initScript(Script& script);

		void update(float deltaTime);

		bool isEnding();
		bool finish(float deltaTime);
		void start(float deltaTime, float introTime);

		void play();
		void pause();
		void stop();

		void setCallback(CallbackType type, SolFunction_t callback);

		void setLoop(bool loop);
		bool getLoop() const;

		float getVolume() const;
		void setVolume(float volume);

		sf::SoundSource::Status getStatus() const;

		float getFadeTime() const;
		float getDuration() const;

		float getTimeLeft() const;
		void setTimeLeft(float timeLeft);

		const std::string& getName() const;

		RatMusic* getSource() const;

		template <typename T>
		T& getEffect() {
			return _base->getEffect<T>();
		}
		
		template <typename T>
    	void cleanEffect() {
			_base->cleanEffect<T>();
		}

	private:

		void reset();
		void loadMusic();
		void getJsonData();

		void callback(CallbackType type);

	};
}
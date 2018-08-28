local tests = {}
local Counter = function()
    
    local self = {
        count = 0
    }

    function self.get()
        self.count = self.count + 1
        return self.count
    end

    return self
end

local counter = Counter()

-- [ Music ]:

tests[counter.get()] = function()

    
    local self  = {
        name = "[M] Getting music playlist from JSON"
    }

    function self.run()
        Music.loadPlaylistFromJson("Assets/Music/Playlists/Test Playlist.json")
        if not Music.includes("Test Playlist", "Rozpierdalacz") or not Music.includes("Test Playlist", "Slowianski") then
            return false
        end
        return true
    end

    function self.clean()
        Music.removeFromPlaylist("Test Playlist") -- funkcja removeFromPlaylist wywołana tylko z 1 argumentem (nazwa playlisty) usuwa całą podaną playliste
    end

    return self

end

tests[counter.get()] = function()

    local self  = {
        name = "[M] Creating music playlist"
    }

    function self.run()
        Music.addPlaylist("Playlist", "Rozpierdalacz") -- Tworzenie nowiej playlisty        
        if not Music.includes("Playlist", "Rozpierdalacz") or Music.includes("Playlist", "Slowianski") then
            return false
        end
        Music.addToPlaylist("Playlist", "Slowianski") -- Dodawanie utworu do juz utworzonej wcześniej playlisty
        if not Music.includes("Playlist", "Rozpierdalacz") or not Music.includes("Playlist", "Slowianski") then
            return false
        end
        Music.removeFromPlaylist("Playlist", "Rozpierdalacz")
        if Music.includes("Playlist", "Rozpierdalacz") or not Music.includes("Playlist", "Slowianski") then
            return false
        end
        return true
    end

    function self.clean()
        Music.removeFromPlaylist("Playlist")
    end

    return self

end

tests[counter.get()] = function()

    local self  = {
        name = "[M] Getting music Status"
    }

    function self.run()
        Music.loadPlaylistFromJson("Assets/Music/Playlists/Test Playlist.json")
        if not Music.getStatus("Test Playlist") == Music.Stopped then 
            return false
        end
        Music.play(0, "Test Playlist") -- 1 argument oznacza ścieszkę na ktorej dana playlista jest odtwarzana, 2 to playlista, mozliwy jest jeszcze 1 czyli nazwa konkretnego utwóru z playlisty
        if not Music.getStatus("Test Playlist") == Music.Playing then 
            return false
        end
        Music.pause(0)
        if not Music.getStatus("Test Playlist") == Music.Paused then 
            return false
        end
        Music.stop(0)
        if not Music.getStatus("Test Playlist") == Music.Stopped then 
            return false
        end
        return true
    end

    function self.clean()
        Music.removeFromPlaylist("Test Playlist")
    end

    return self

end

tests[counter.get()] = function()

    local self  = {
        name = "[M] Setting music Playing Mode"
    }

    -- Logs
    function self.run()
        Music.loadPlaylistFromJson("Assets/Music/Playlists/Test Playlist.json")
        Music.setPlayingMode("Test Playlist", Music.Single)
        Music.setPlayingMode("Test Playlist", Music.Random)
        Music.setPlayingMode("Test Playlist", Music.Orderly)
        return true
    end

    function self.clean()
        Music.removeFromPlaylist("Test Playlist")
    end

    return self

end

tests[counter.get()] = function()

    local self  = {
        name = "[M] Getting and setting music Volume"
    }

    function self.run()
        Music.loadPlaylistFromJson("Assets/Music/Playlists/Test Playlist.json")
        Music.setVolume(50)
        if not Music.getVolume() == 50 or not Music.getVolume("Test Playlist") == 100 or not Music.getVolume("Test Playlist", "Rozpierdalacz") == 100 then
            return false
        end
        Music.setVolume(50, "Test Playlist")
        if not Music.getVolume() == 50 or not Music.getVolume("Test Playlist") == 50 or not Music.getVolume("Test Playlist", "Rozpierdalacz") == 100 then
            return false
        end
        Music.setVolume(50, "Test Playlist", "Rozpierdalacz")
        if not Music.getVolume() == 50 or not Music.getVolume("Test Playlist") == 50 or not Music.getVolume("Test Playlist", "Rozpierdalacz") == 50 then
            return false
        end
        return true
    end

    function self.clean()
        Music.setVolume(100)
        Music.removeFromPlaylist("Test Playlist")
    end

    return self

end

tests[counter.get()] = function()

    local self  = {
        name = "[M] Adding music in various formats"
    }

    function self.run()
        -- Zeldowy (wav)
        -- Rozpierdalacz (flac)
        Music.addPlaylist("Playlist", "Zeldowy", "Rozpierdalacz")        
        if not Music.includes("Playlist", "Zeldowy") or not Music.includes("Playlist", "Rozpierdalacz") then
            return false
        end
        return true
    end

    function self.clean()
        Music.removeFromPlaylist("Playlist")
    end

    return self

end

tests[counter.get()] = function()

    local self  = {
        name = "[M] Getting independent music object"
    }

    function self.run()
        local music = Music.loadMusic("Rozpierdalacz")
        if music == nil then
            return false
        end
        Music.removeMusic(music)
        if not music == nil then
            return false
        end
        return true
    end

    function self.clean()
    end

    return self

end

tests[counter.get()] = function()

    local self  = {
        name = "[M] Using independent music obiect",
        testMusic = nil
    }

    function self.run()
        self.testMusic = Music.loadMusic("Rozpierdalacz")
        if self.testMusic == nil then
            return false
        end
        self.testMusic:setVolume(self.testMusic:getVolume() * 0.8)
        self.testMusic:setLoop(not self.testMusic:getLoop())
        if not self.testMusic:getStatus() == Music.Stopped then
            return false
        end
        if not self.testMusic:getName() == "Rozpierdalacz" then
            return false
        end
        return true
    end

    function self.clean()
        Music.removeMusic(self.testMusic)
    end

    return self

end

-- [ SOUND ]:

tests[counter.get()] = function()

    local self  = {
        name = "[S] Adding and Removing Sound without entity"
    }

    -- Logs
    function self.run()
        local sound = Sound.addNewSound("Button");
        if sound == nil then
            return false
        end
        Sound.removeSound(sound);
        if not sound == nil then
            return false
        end
        return true
    end
    
    function self.clean()
    end

    return self

end

tests[counter.get()] = function()

    local self = {
        name = "[S] Setting and getting sound Volume",
        testSound = nil
    }

    function self.run()
        self.testSound = Sound.addNewSound("Button");
        if self.testSound == nil then
            return false
        end
        Sound.setVolume(50)
        if not Sound.getVolume() == 50 or not self.testSound:getVolume() == 100 then
            return false
        end
        self.testSound:setVolume(50)
        if not Sound.getVolume() == 50 or not self.testSound:getVolume() == 50 then
            return false
        end
        return true
    end
    
    function self.clean()
        Sound.setVolume(100)
        Sound.removeSound(self.testSound);
    end

    return self

end

tests[counter.get()] = function()

    local self = {
        name = "[S] Getting sound Status",
        testSound = nil
    }

    function self.run()
        self.testSound = Sound.addNewSound("Button");
        if self.testSound == nil then
            return false
        end
        if not self.testSound:getStatus() == Sound.Stopped then 
            return false
        end
        self.testSound:play()
        if not self.testSound:getStatus() == Sound.Playing then 
            return false
        end
        self.testSound:pause()
        if not self.testSound:getStatus() == Sound.Paused then 
            return false
        end
        self.testSound:stop()
        if not self.testSound:getStatus() == Sound.Stopped then 
            return false
        end
        return true
    end
    
    function self.clean()
        Sound.removeSound(self.testSound);
    end

    return self

end

tests[counter.get()] = function()

    local self = {
        name = "[S] Control of all sounds",
        sound_1 = nil,
        sound_2 = nil
    }

    function self.run()
        self.sound_1 = Sound.addNewSound("Button");
        self.sound_2 = Sound.addNewSound("Button 1");
        if self.sound_1 == nil or self.sound_2 == nil then
            return false
        end
        Sound.play()
        if not self.sound_1:getStatus() == Sound.Playing or not self.sound_2:getStatus() == Sound.Playing then 
            return false
        end
        Sound.pause()
        if not self.sound_1:getStatus() == Sound.Paused or not self.sound_2:getStatus() == Sound.Paused then 
            return false
        end
        self.sound_1:play()
        Sound.stop()
        if not self.sound_1:getStatus() == Sound.Stopped or not self.sound_2:getStatus() == Sound.Stopped then 
            return false
        end
        return true
    end
    
    function self.clean()
        Sound.removeSound(self.sound_1);
        Sound.removeSound(self.sound_2);
    end

    return self

end

tests[counter.get()] = function()

    local self = {
        name = "[S] Control of sound position",
        testSound = nil
    }

    function self.run()
        self.testSound = Sound.addNewSound("Button");
        if self.testSound == nil then
            return false
        end
        local pos = self.testSound:getPosition()
        if not pos.x == 0 or not pos.y == 0 or not pos.z == 0 then
            return false
        end
        self.testSound:setPosition(10, 20.1, 30.5)
        pos = self.testSound:getPosition()
        if not pos.x == 10 or not pos.y == 20.1 or not pos.z == 30.5 then
            return false
        end
        return true
    end
    
    function self.clean()
        Sound.removeSound(self.testSound);
    end

    return self

end

tests[counter.get()] = function()

    local self = {
        name = "[S] Load sound directly form file",
        testSound = nil
    }

    function self.run()
        self.testSound = Sound.addNewSound("Assets/Sounds/button.flac");
        if self.testSound == nil then
            return false
        end
        return true
    end
    
    function self.clean()
        Sound.removeSound(self.testSound);
    end

    return self

end

tests[counter.get()] = function()

    local self = {
        name = "[S] Sound division",
        testSound = nil
    }

    function self.run()
        self.testSound = Sound.addNewSound("Assets/Sounds/button.flac");
        if self.testSound == nil then
            return false
        end
        local length = self.testSound:getLength() -- ok 420ms -- {Pobranie długosci całego dzwięku}
        self.testSound:setOffset(0.10, length) -- 100ms - ok 420ms
        local duration = self.testSound:getEndTime() - self.testSound:getBeginTime() -- {Pobranie długosci aktualnego sampla}
        if not Math.floor(duration * 100) == 32 then -- Dzwięk powinien twać około 320ms
            return false
        end
        return true
    end
    
    function self.clean()
        Sound.removeSound(self.testSound);
    end

    return self

end

tests[counter.get()] = function()

    local self = {
        name = "[S] Sound Callbacks",
        testSound = nil
    }

    function self.run()
        local started = false
        local finished = false
        self.testSound = Sound.addNewSound("Button");
        if self.testSound == nil then
            return false
        end
        function self.testSound.onStart()
            started = true
        end
        function self.testSound.onFinish()
            finished = true
        end
        self.testSound:play()
        self.testSound:stop()
        if not started or not finished then
            return false
        end
        return true
    end
    
    function self.clean()
        Sound.removeSound(self.testSound);
    end

    return self

end

------------------------------------------------------------------------------------------

local counter = {
    all = 0,
    passed = 0,
    failed = 0
}

local waitForEnter = function()
    io.stdin:read'*l'
end

-- Do all tests
print("---------------------------------------")
for key, test in pairs(tests) do
    counter.all = counter.all + 1

    local t = test()
    
    print("Current Test:", t.name)
    
    local res = t:run()

    print("Result:", res)

    t:clean()

    if res == false then
        counter.failed = counter.failed + 1
        waitForEnter() 
    else
        counter.passed = counter.passed + 1
    end


    print("---------------------------------------")
end

print("Result of tests:")
print("All:", counter.all)
print("Passed:", counter.passed)
print("Failed:", counter.failed)
print("---------------------------------------")

waitForEnter()
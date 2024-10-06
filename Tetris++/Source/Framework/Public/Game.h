#pragma once

#include "Renderer.h"
#include "Event.h"

#include <string>
#include <memory>

class GameMode;
class Renderer;
class InputHandler;
struct GLFWwindow;

class Game
{
public:
    static Game& GetGameInstance()
    {
        static Game game;
        return game;
    }

private:
    std::string m_GameName = "Tetris++";
    int m_WindowWidth = 0;
    int m_WindowHeight = 0;

    std::shared_ptr<Renderer> m_Renderer;
    std::shared_ptr<InputHandler> m_InputHandler;
    GLFWwindow* m_Window;

    Event<float> m_UpdateEvent;

private:
    Game();

public:
    Game(Game const&) = delete;
    void operator=(Game const&) = delete;

private:
    std::shared_ptr<GameMode> m_GameMode;

public:
    inline std::shared_ptr<GameMode> GetGameMode() const { return m_GameMode; }

private:
    bool b_RunGame = true;

public:
    void Run(const std::string& arguments);

    inline InputHandler* GetInputHandler() { return m_InputHandler.get(); }
    inline Renderer* GetRenderer() { return m_Renderer.get(); }

    inline Event<float>& GetUpdateEvent() { return m_UpdateEvent; }

    void StopGame();
};

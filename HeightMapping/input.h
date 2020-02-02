#pragma once

#include <Qt>
#include <QPoint>

class Input
{
public:
    enum class State
    {
        Invalid,
        Registered,
        Unregistered,
        Triggered,
        Pressed,
        Released
    };

    static State keyState( Qt::Key key);
    static bool keyTriggered( Qt::Key key);
    static bool keyPressed( Qt::Key key);
    static bool keyReleased( Qt::Key key);
    static State buttonState( Qt::MouseButton button );
    static bool buttonTriggered( Qt::MouseButton button );
    static bool buttonPressed( Qt::MouseButton button );
    static bool buttonReleased( Qt::MouseButton button );
    static QPoint mousePosition();
    static QPoint mouseDelta();

private:
    static void update();
    static void registerKeyPress( int key );
    static void registerKeyRelease( int key );
    static void registerMousePress( Qt::MouseButton button );
    static void registerMouseRelease( Qt::MouseButton button );
    static void reset();

    friend class MainWindow;
};

inline bool Input::keyTriggered(Qt::Key key)
{
  return keyState(key) == State::Triggered;
}

inline bool Input::keyPressed(Qt::Key key)
{
  return keyState(key) == State::Pressed;
}

inline bool Input::keyReleased(Qt::Key key)
{
  return keyState(key) == State::Released;
}

inline bool Input::buttonTriggered(Qt::MouseButton button)
{
  return buttonState(button) == State::Triggered;
}

inline bool Input::buttonPressed(Qt::MouseButton button)
{
  return buttonState(button) == State::Pressed;
}

inline bool Input::buttonReleased(Qt::MouseButton button)
{
  return buttonState(button) == State::Released;
}

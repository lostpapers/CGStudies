#include <QCursor>
#include <vector>
#include <algorithm>

#include "input.h"

template <typename T>
struct InputInstance : std::pair<T, Input::State>
{
  typedef std::pair<T, Input::State> base_class;

  inline InputInstance(T value) : base_class(value, Input::State::Invalid)
  {}

  inline InputInstance(T value, Input::State state) : base_class(value, state)
  {}

  inline bool operator==(const InputInstance &rhs) const
  {
    return this->first == rhs.first;
  }
};

// Instance types
typedef InputInstance<Qt::Key> KeyInstance;
typedef InputInstance<Qt::MouseButton> ButtonInstance;

// Container types
typedef std::vector<KeyInstance> KeysContainer;
typedef std::vector<ButtonInstance> ButtonsContainer;

// Globals
static KeysContainer sg_keyInstances;
static ButtonsContainer sg_buttonInstances;
static QPoint sg_mouseCurrentPosition;
static QPoint sg_mousePreviousPosition;
static QPoint sg_mouseDelta;



static inline KeysContainer::iterator FindKey( Qt::Key key)
{
    return std::find(sg_keyInstances.begin(),sg_keyInstances.end(), key);
}

static inline ButtonsContainer::iterator FindButton( Qt::MouseButton button)
{
    return std::find(sg_buttonInstances.begin(),sg_buttonInstances.end(), button);
}

template <typename TPair>
static inline void UpdateStates(TPair &instance)
{
  switch (instance.second)
  {
  case Input::State::Registered:
    instance.second = Input::State::Triggered;
    break;
  case Input::State::Triggered:
    instance.second = Input::State::Pressed;
    break;
  case Input::State::Unregistered:
    instance.second = Input::State::Released;
    break;
  default:
    break;
  }
}

template <typename TPair>
static inline bool CheckReleased(const TPair &instance)
{
  return instance.second == Input::State::Released;
}

template <typename Container>
static inline void Update(Container &container)
{
  typedef typename Container::iterator Iter;
  typedef typename Container::value_type TPair;

  // Remove old data
  Iter remove = std::remove_if(container.begin(), container.end(), &CheckReleased<TPair>);
  container.erase(remove, container.end());

  // Update existing data
  std::for_each(container.begin(), container.end(), &UpdateStates<TPair>);
}

Input::State Input::keyState(Qt::Key k)
{
  KeysContainer::iterator it = FindKey(k);
  return (it != sg_keyInstances.end()) ? it->second : State::Invalid;
}

Input::State Input::buttonState(Qt::MouseButton k)
{
  ButtonsContainer::iterator it = FindButton(k);
  return (it != sg_buttonInstances.end()) ? it->second : State::Invalid;
}

QPoint Input::mousePosition()
{
  return QCursor::pos();
}

QPoint Input::mouseDelta()
{
  return sg_mouseDelta;
}

void Input::update()
{
  // Update Mouse Delta
  sg_mousePreviousPosition = sg_mouseCurrentPosition;
  sg_mouseCurrentPosition = QCursor::pos();
  sg_mouseDelta = sg_mouseCurrentPosition - sg_mousePreviousPosition;

  // Update KeyState values
  Update(sg_buttonInstances);
  Update(sg_keyInstances);
}

void Input::registerKeyPress(int k)
{
  KeysContainer::iterator it = FindKey((Qt::Key)k);
  if (it == sg_keyInstances.end())
  {
    sg_keyInstances.push_back(KeyInstance((Qt::Key)k, State::Registered));
  }
}

void Input::registerKeyRelease(int k)
{
  KeysContainer::iterator it = FindKey((Qt::Key)k);
  if (it != sg_keyInstances.end())
  {
    it->second = State::Unregistered;
  }
}

void Input::registerMousePress(Qt::MouseButton btn)
{
  ButtonsContainer::iterator it = FindButton(btn);
  if (it == sg_buttonInstances.end())
  {
    sg_buttonInstances.push_back(ButtonInstance(btn, State::Registered));
  }
}

void Input::registerMouseRelease(Qt::MouseButton btn)
{
  ButtonsContainer::iterator it = FindButton(btn);
  if (it != sg_buttonInstances.end())
  {
    it->second = State::Unregistered;
  }
}

void Input::reset()
{
  sg_keyInstances.clear();
  sg_buttonInstances.clear();
}

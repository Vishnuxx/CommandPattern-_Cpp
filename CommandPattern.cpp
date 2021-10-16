#include <stack>
#include <iostream>
#include <memory>

// ----- the Command Interface -----
class Command
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

// ----- the MODEL -----
class Fan
{

    bool on;
    int speed;

public:
    Fan() {}

    void switchOn()
    {
        on = true;
    }

    void switchOff()
    {
        on = false;
    }

    void setSpeed(int speed)
    {
        speed = speed;
    }

    bool isOn()
    {
        return on;
    }

    int getSpeed()
    {
        return speed;
    }
};

// ----- concrete ICommand commands -----
class FanOnCommand : public Command
{

    Fan *mFan;

public:
    FanOnCommand(Fan &fan) : mFan(&fan) {}

    void execute()
    {
        mFan->switchOn();
    }

    void undo()
    {
        mFan->switchOff();
    }

    void redo()
    {
        mFan->switchOn();
    }
};

//set fan speed command

class FanSpeedCommand : public Command
{

    Fan *mFan;
    int newSpeed;
    int prevSpeed;

public:
    FanSpeedCommand(Fan *fan, int speed)
    {
        mFan = fan;
        newSpeed = speed;
    }

    void execute()
    {
        prevSpeed = mFan->getSpeed();
        mFan->setSpeed(newSpeed);
    }

    void undo()
    {
        mFan->setSpeed(prevSpeed);
    }

    void redo()
    {
        mFan->setSpeed(newSpeed);
    }
};

typedef std::stack<std::shared_ptr<Command> > commandStack_t;

class HistoryManager
{

    commandStack_t undoStack;
    commandStack_t redoStack;

public:
    HistoryManager() {}

    void executeCmd(std::shared_ptr<Command> command)
    {
        redoStack = commandStack_t();
        command->execute();
        undoStack.push(command);
    }

    void undo()
    {
        if (undoStack.size() <= 0)
        {
            return;
        }
        undoStack.top()->undo();
        redoStack.push(undoStack.top());
        undoStack.pop();
    }

    void redo()
    {
        if (redoStack.size() <= 0)
        {
            return;
        }
        redoStack.top()->redo();
        undoStack.push(redoStack.top());
        redoStack.pop();
    }
};

int main()
{
    using namespace std;

    Fan fan;
    HistoryManager history;

    std::shared_ptr<Command> command1(new FanSpeedCommand(&fan, 1)); // create command for switching to channel 1
    history.executeCmd(command1);
    std::cout << "set fan speed to: " << fan.getSpeed() << std::endl;

    std::shared_ptr<Command> command2(new FanSpeedCommand(&fan, 2)); // create command for switching to channel 2
    history.executeCmd(command2);
    std::cout << "set fan speed to: " << fan.getSpeed() << std::endl;

    std::shared_ptr<Command> command3(new FanSpeedCommand(&fan, 3)); // create command for switching to channel 3
    history.executeCmd(command3);
    std::cout << "set fan speed to: " << fan.getSpeed() << std::endl;

    std::cout << "undo performed" << std::endl;
    history.undo();
    std::cout << "current speed: " << fan.getSpeed() << std::endl;

    std::cout << "undo performed" << std::endl;
    history.undo();
    std::cout << "current speed: " << fan.getSpeed() << std::endl;

    std::cout << "redo performed" << std::endl;
    history.redo();
    std::cout << "current speed: " << fan.getSpeed() << std::endl;

    std::cout << "redo performed" << std::endl;
    history.redo();
    std::cout << "current channel: " << fan.getSpeed() << std::endl;

    return 0;
}
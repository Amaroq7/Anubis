![Anubis](https://anubis-static-content.s3.eu-central-1.amazonaws.com/logo/anubis_dark.png "Anubis")

# What is Anubis?

Anubis is a plugin/DLL manager that sits between the Half-Life Engine and an HL Game mod, allowing the mod-like DLL
plugins to add functionality to the HL server or game mod. Its goal is to provide C++ API to interact with the engine as
well as the game library.

## Requirements

- [ReHLDS](https://github.com/dreamstalker/rehlds) v3.11+
- [ReGameDLL_CS](https://github.com/s1lentq/ReGameDLL_CS) v5.21+ (cstrike & czero)

## Game support

|                      Game                       | Basic | VFunc hooking |
|:-----------------------------------------------:|:-----:|:-------------:|
|                    Half-Life                    |   ✔   |       ✔       |
| Counter-Strike / Counter-Strike: Condition Zero |   ✔   |       ✔       |
|              Team Fortress Classic              |   ✔   |       ✖       |
|                  Day Of Defeat                  |   ✔   |       ✖       |

## Example plugin

```cpp
#include <IAnubis.hpp>
#include <ILogger.hpp>
#include <game/ILibrary.hpp>
#include <engine/ILibrary.hpp>

class Plugin : public Anubis::IPlugin
{
public:
    Anubis::InterfaceVersion getInterfaceVersion() const
    {
        return Anubis::IAnubis::VERSION;
    }
    
    std::string_view getName() const
    {
        return "Minimal plugin";
    }
    
    std::string_view getVersion() const
    {
        return "1.0";
    }
    
    std::string_view getDate() const
    {
        return __DATE__;
    }
    
    std::string_view getAuthor() const
    {
        return "Anubis Dev Team";
    }
    
    std::string_view getUrl() const
    {
        return "https://github.com/Amaroq7/Anubis";
    }
    
    Type getType() const
    {
        return Type::Extension;
    }
};

nstd::observer_ptr<Anubis::IAnubis> gApi;
nstd::observer_ptr<Anubis::Game::ILibrary> gGame;
nstd::observer_ptr<Anubis::Engine::ILibrary> gEng;
std::unique_ptr<Anubis::ILogger> gLogger;

/* The following functions are exported from the plugin */
namespace Anubis
{
    nstd::observer_ptr<IPlugin> Query()
    {
        static auto pluginInfo = std::make_unique<Plugin>();
        return pluginInfo;
    }

    bool Init(nstd::observer_ptr<IAnubis> api)
    {
        gApi = api;
        gGame = gApi->getGame(Game::ILibrary::VERSION);
        gEng = gApi->getEngine(Engine::ILibrary::VERSION);
        
        gLogger = gApi->getLogger(ILogger::VERSION);
        gLogger->setLogTag("TEST");
        gLogger->setLogLevel(LogLevel::Debug);
        
        return true;
    }
    
    void Shutdown()
    { 
        gLogger.reset();
    }
}
```
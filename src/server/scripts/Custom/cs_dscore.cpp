#include "ScriptPCH.h"
#include "Chat.h"

class dscore_commands : public CommandScript
{
    public:
        dscore_commands() : CommandScript("dscore_commands") { }

        static bool HandleInfoCommand(ChatHandler* handler, const char* /*args*/)
        {
            handler->PSendSysMessage("DarkShock Command Script Version 0.1 Alpha");
            return true;
        }

        ChatCommand* GetCommands() const
        {
            static ChatCommand dsCommandTable[] =
            {
                { "info", SEC_MODERATOR, true, &HandleInfoCommand, "", NULL },
                { NULL, 0, false, NULL, "", NULL }
            };
            
            static ChatCommand commandTable[] =
            {
                { "ds", SEC_MODERATOR, true, NULL, "", dsCommandTable},
                { NULL, 0, false, NULL, "", NULL}
            };
            
            return commandTable;
        }
};

void AddSC_dscore_commands()
{
    new dscore_commands();
}

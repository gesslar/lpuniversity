/* makedev.c

 Tacitus @ LPUniversity
 14-JAN-06
 Admin Command

*/

/* Last edited on October 6th, 2006 by Tacitus */

#include <logs.h>

int main(string args)
{
     object user;
     object security_editor;

     if(!adminp(previous_object())) return notify_fail("Error [makedev]: Access denied.\n");

     if(!args) return(notify_fail("Syntax: makedev <user>\n"));
     args = lower_case(args);
     user = find_player(args);

     if(!user) return(notify_fail("Error [makedev]: User '" + args + "' is not online.\n"));
     if(devp(query_privs(user)))
     {
         write("Error [makedev]: That user is already a developer.\n");
         write("Setting up command path for '" + user->query_cap_name() + "'.\n");
         user->add_path("/cmds/wiz/");
         user->add_path("/cmds/object/");
            user->add_path("/cmds/file/");
            return 1;
        }

     write("Setting up developer access for '" + capitalize(user->query_name()) + "'.\n");

     tell_object(user, "\nSetting up developer account...\n");
     if(!directory_exists("/home/" + user->query_name()[0..0])) mkdir ("/home/"
         + user->query_name()[0..0]);
     mkdir("/home/" + user->query_name()[0..0] + "/" + user->query_name());
     mkdir("/home/" + user->query_name()[0..0] + "/" + user->query_name() + "/public");
     mkdir("/home/" + user->query_name()[0..0] + "/" + user->query_name() + "/private");
     cp("/areas/std/workroom.c", user_path(user->query_name()));
     write_file("/doc/journals/journal." + user->query_name(),
         capitalize(user->query_name()) + "'s dev journal (Created: " + ctime(time()) + ")\n\n");
     catch(link("/doc/journals/journal." + user->query_name(),
         "/home/" + user->query_name()[0..0] + "/" + user->query_name() + "/private/journal."
         + user->query_name()));
     user->add_path("/cmds/wiz/");
     user->add_path("/cmds/object/");
     user->add_path("/cmds/file/");
     security_editor = clone_object("/adm/obj/security_editor.c");
     security_editor->enable_membership(query_privs(user), "developer");
     security_editor->write_state(0);
     tell_object(user, "\n... Success!\n\n");
     tell_object(user, "%^BOLD%^GREEN%^Developer Access Granted.%^RESET%^\n");

     write("Success [makedev]: User '" + capitalize(user->query_name()) + "' now has developer status.\n");
     log_file(LOG_PROMOTE, capitalize(query_privs(this_player())) + " promotes " 
        + user->query_cap_name() + " to developer status on " + ctime(time())
        + "\n");
     return 1;
}

string help()
{
     return(" %^BOLD%^SYNTAX:%^RESET%^ makedev <user>\n"
     "This command will set up a user with developer access to\n"
     "the mud by adding the user to the 'developer' group, creating\n"
     "them a home directory, and adding '/cmds/wiz/', '/cmds/object/',\n"
     "and '/cmds/file/' to the user's directory.\n\n"
     "%^BOLD%^See also:%^RESET%^ revdev\n");
}

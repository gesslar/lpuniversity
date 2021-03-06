/* reply.c

 Tacitus @ LPUniversity
 05-MAY-06
 Std reply command
 
*/

//Last edited on August 11th, 2006 by Parthenon
//Last edited on September 10th, 2006 by Tricky
//Last edited on September 18th, 2006 by Tricky

int main(string message)
{
    object user;
    string d_user, d_mud;
    string who = this_player()->query("reply");

    if(!message) return(notify_fail("Error [tell]: You must provide an argument. Syntax: reply <message>\n"));

    /* Reply to I3 user */
    if(sscanf(who, "%s@%s", d_user, d_mud))
    {
        object I3;

        if(lower_case(d_user) == lower_case(query_privs(this_player())) &&
           lower_case(d_mud) == lower_case(mud_name()))
        {

            if(message[0] == ':') message = message[1..];

            write(CYN + "You reply to yourself: " + NOR + message + "\n" + NOR);
            tell_room(environment(this_player()), this_player()->query_cap_name() + " starts talking to themselves.\n" + NOR, this_player());

            return 1;
        }

        I3 = load_object("/adm/daemons/chmodules/chdmod_i3.c");

        if(message[0] == ':')
        {
            I3->send_emoteto(this_player(), d_user, d_mud, message[1..]);
            write(CYN + "You reply to " + capitalize(d_user) + "@" + d_mud + ": " + NOR + this_player()->query_cap_name() + " " + message[1..] + "\n" + NOR);
        }
        else
        {
            I3->send_tell(this_player(), d_user, d_mud, message);
            write(CYN + "You reply to " + capitalize(d_user) + "@" + d_mud + ": " + NOR + message + "\n" + NOR);
        }

        return 1;
    }

    user = find_player(who);
    
    if(!objectp(user)) return notify_fail("Error [tell]: User " + who + " is not found.\n");
    
    if(user == this_player())
    {

        if(message[0] == ':') message = message[1..];

        write(CYN + "You reply to yourself: " + NOR + message + "\n" + NOR);
        tell_room(environment(this_player()), this_player()->query_cap_name() + " starts talking to themselves.\n" + NOR, this_player());

        return 1;
    }

    if(message[0] == ':')
    {
        tell_object(user, CYN + this_player()->query_cap_name() + " " + message[1..] + "\n" + NOR);
        write(CYN + "You reply to " + capitalize(who) + ": " + NOR + this_player()->query_cap_name() + " " + message[1..] + "\n" + NOR);
    }
    else
    {
        tell_object(user, CYN + this_player()->query_cap_name() + " tells you: " + NOR + message + "\n" + NOR);
        write(CYN + "You reply to " + capitalize(who) + ": " + NOR + message + "\n" + NOR);
    }

    user->set("reply", query_privs(this_player()));

    return 1;
}

string help()
{
    return(HIW + " SYNTAX:" + NOR + " reply <message>\n\n"
    "This command will send a message to the last user that sent\n"
    "you a message if they are still online.\n\n"
    HIW + "See also: " + NOR + "say, channel\n");
}

/* valid.c

 Tacitus @ LPUniversity
 23-APR-05
 Valid hooks, inherited by Master Obj

 Refractored 21-DEC-05 by Tacitus @ LPUni

*/

/* Last edited on July 17th, 2006 by Tacitus */

/* Preproccessor Statements */

#define FILE_GROUPDATA "/adm/etc/groups"
#define FILE_ACCESSDATA "/adm/etc/access"
//#define DEBUG

/* Global Variable */

mapping access = ([]);
mapping groups = ([]);

/* Function prototypes */

void parse_group();
void parse_access();
string *parse(string str);
string *query_group(string group);
string *track_member(string id, string directory);
int query_access(string directory, string id, int type);
int isMember(string user, string group);

/* Functions */

void create()
{
    parse_group();
    parse_access();
}

void parse_group()
{
    int i, n;
    string *arr = parse(read_file(FILE_GROUPDATA));

#ifdef DEBUG

    write_file("/log/security", "\tDebug [security]: Parsing group data file...\n");

#endif

    groups = ([]);

    for(i = 0; i < sizeof(arr); i++)
    {
    string group, str, *members;

    if(!arr[i]) continue;

    if(sscanf(arr[i], "(%s)%s", group, str) != 2)
    {
        write("Error [security]: Invalid format of data in group data.\n");
        write("Security alert: Ignoring group on line " + (i + 1) + "\n");
        continue;
    }

    members = explode(str, ":");

#ifdef DEBUG

    write_file("/log/security", "Debug [security]: Adding group '" + group + "' with " + sizeof(members) + " members.\n");

#endif

    for(n = 0; n < sizeof(members); n++)
    {
        if(!file_size(user_data_file(members[n])) && !sscanf(members[n], "[%*s]"))
        {
        write("Error [security]: Unknown user detected.\n");
        write("Security alert: User '" + members[n] + "' ignored for group '" + group + "'.\n");
        members -= ({ members[n] });
        continue;
        }
#ifdef DEBUG

        write_file("/log/security", "Debug [security]: Adding user '" + members[n] + "' to group '" + group + "'.\n");

#endif
    }

    groups += ([group : members]);
    }
}

void parse_access()
{
    int i, n;
    string *arr = parse(read_file(FILE_ACCESSDATA));

#ifdef DEBUG

    write_file("/log/security", "\tDebug [security]: Parsing access data file...\n");

#endif

    access = ([]);

    for(i = 0; i < sizeof(arr); i++)
    {
    string directory, str, *entries;
    mapping data;

    data = ([]);

    if(!arr[i]) continue;

    if(sscanf(arr[i], "(%s)%s", directory, str) != 2)
    {
        write("Error [security]: Invalid format of data in access data.\n");
        error("Security alert: Fatal error parsing access data on line " + (i + 1) + "\n");
    }

    if(str[<1..< 1] == ":")
    {
        write("Error [security]: Incomplete data in access data (trailing ':').\n");
        error("Security alert: Fatal error parsing access data on line " + (i + 1) + "\n");
    }

    entries = explode(str, ":");

#ifdef DEBUG

    write_file("/log/security", "Debug [security]: Parsing data for directory '" + directory + "'.\n");

#endif

    for(n = 0; n < sizeof(entries); n++)
    {
        string identity, permissions, *permArray = allocate(8);
        if(sscanf(entries[n], "%s[%s]", identity, permissions) != 2)
        {
        write("Error [security]: Invalid entry(" + n + ") data format in access data.\n");
        error("Security alert: Fatal error parsing access data on line " + (i + 1) + "\n");
        }

#ifdef DEBUG

        write_file("/log/security", "Debug [security]: Adding identity '" + identity + "' with permission string of '" + permissions + "'.\n");

#endif

        //read, write, network, shadow, link, execute, bind, ownership
        if(strsrch(permissions, "r") != -1) permArray[0] = "r";
        if(strsrch(permissions, "w") != -1) permArray[1] = "w";
        if(strsrch(permissions, "n") != -1) permArray[2] = "n";
        if(strsrch(permissions, "s") != -1) permArray[3] = "s";
        if(strsrch(permissions, "l") != -1) permArray[4] = "l";
        if(strsrch(permissions, "e") != -1) permArray[5] = "e";
        if(strsrch(permissions, "b") != -1) permArray[6] = "b";
        if(strsrch(permissions, "o") != -1) permArray[7] = "o";

        data += ([ identity : permArray ]);

    }

    access += ([directory : data]);
    }
}

string *parse(string str)
{
    string *arr;
    int i;

    if (!str) {
        return ({});
    }

    arr = explode(str, "\n");

    for (i = 0; i < sizeof(arr); i++) 
    {
        if (arr[i][0] == '#') 
        {
            arr[i] = 0;
            continue;
        }
        arr[i] = replace_string(arr[i], " ", "");
        arr[i] = replace_string(arr[i], "\t", "");
    }
    
    return arr;
}

int valid_shadow(object ob)
{
    string location, name;
    location = get_base(ob);
    name = query_privs(ob);
    if(ob == this_object() || ob == master()) return 0;
    if(query_access(location, name, 4) && !ob->disallow_shadow(ob)) return 1;
    return 0;
}

int valid_bind(object obj, object owner, object victim)
{
    string name;
    name = query_privs(previous_object());
    if(query_access(get_base(owner), name, 7) && query_access(get_base(victim), name, 7)) return 1;
    return 0;
}

int valid_compile_to_c()
{
    return 0;
}

int valid_hide(object ob)
{
    return 0;
}

int valid_link(string from, string to)
{
    string name;
    if(this_interactive()) name = query_privs(this_interactive());
    else name = query_privs(previous_object());
    if(query_access(from, name, 5) && query_access(to, name, 5))
    return 1;
    return 0;
}

int valid_object(object ob)
{
    string location, name;
    location = file_name(ob);
    if(this_interactive()) name = query_privs(this_interactive());
    else name = query_privs(previous_object());
    if(!name) name = "noname";
    if(file_name(ob) == "/adm/daemons/login") return 1;
    if(query_access(location, name, 6)) return 1;
    return 0;
}

int valid_override(string file, string efun_name, string mainfile)
{

    if (mainfile == "/adm/obj/simul_efun.c") {
    return 1;
    }
    if(efun_name == "destruct" && mainfile == "/std/object/object.c") return 1;
    if(efun_name == "ed") return 1;
    return 0;
}

int valid_socket(object caller, string func, mixed *info)
{
    //We might code a daemon or something that allows
    //us to ban connections to certain ports/connections

    /* string name;
    name = query_privs(caller);
    if(query_access(get_base(info[1]), name, 3)) return 1;
    return 0;  */

    return 1;
}

int valid_read(string file, object user, string func)
{
    string name, tmp, tmp2;

    if(this_interactive() && query_privs(user) != "[daemon]")
        name = query_privs(this_interactive());
        
    else name = query_privs(user);
    if(!name) name = "noname";

    if( strlen(file) > strlen(user_data_directory(name)) )
    {
        if( file[0..(strlen(user_data_directory(name))-1)] 
              == user_data_directory(name) ) 
            return 1;
    }

    if(func == "file_size") return 1;
    if(func == "restore_object" && member_array(find_object("/adm/daemons/finger_d.c"), all_previous_objects()) != -1) return 1;


    if(file && sscanf(file, "/home/%*s/%s/%s", tmp, tmp2))
    {
    if(name == tmp || name == "[home_" + tmp + "]") return 1;
    if(tmp2 && tmp2[0..5] == "public") return 1;
    if(tmp2 && tmp2[0..6] == "private" && name == tmp) return 1;
    if(tmp2 && tmp2[0..6] == "private" && name != tmp && !isMember(name, "admin")) return 0;
    }

#ifdef DEBUG
    write_file("/log/security", "Debug [valid_read]: File: " + file + " Name: " + name + "\n");
#endif

    if(query_access(file, name, 1)) return 1;
    return 0;
}

int valid_write(string file, object user, string func)
{
    string name, tmp, tmp2;
    if(this_interactive() && query_privs(user) != "[daemon]")
    name = query_privs(this_interactive());
    else name = query_privs(user);
    if(!name) name = "noname";

    if(user == this_object() || user == master()) return 1;

    if(strlen(file) > strlen(user_data_directory(name)))
    {
        if( file[0..(strlen(user_data_directory(name))-1)] == user_data_directory(name) ) return 1;
    }


    if(file && sscanf(file, "/home/%*s/%s/%s", tmp, tmp2))
    {
        if(name == tmp || name == "[home_" + tmp + "]") return 1;
        if(tmp2 && tmp2[0..6] == "public/" && tmp2 != "public/") return 1;
        if(tmp2 && tmp2[0..6] == "private" && name == tmp) return 1;

    }


#ifdef DEBUG
    write_file("/log/security", "Debug [valid_write]: File: " + file + " Name: " + name + "\n");
#endif
    if(query_access(file, name, 2)) return 1;
    return 0;
}

int query_access(string directory, string id, int type)
{
    mapping data;
    string *permissions;

#ifdef DEBUG

    write_file("/log/security", "Debug [security]: Permission query for '" + id + "' in '" + directory + "'.\n");
#endif

    data = access[directory];

    if(!mapp(data))
    {
        string *exp;
        int size, i;

#ifdef DEBUG
        write_file("/log/security", "Debug [query_access]: " + directory + " not in mapping. Searching for parent permissions.\n");
#endif
        exp = explode(directory, "/");
        size = sizeof(exp);
        while(!access[directory] && size --)
        {

            directory = "/";
            for(i = 0; i < size; i ++)
            {
                directory += exp[i] + "/";
            }
        }

        if(!access[directory]) directory = "/";
    }

    if(!access[directory]) return 0;
    else data = access[directory];

#ifdef DEBUG

    write_file("/log/security", "Debug [query_access]: Final directory set to '" + directory + "'.\n");

#endif


    permissions = data[id];
    if(!permissions || !arrayp(permissions) || sizeof(permissions) < 1) permissions = track_member(id, directory);

    if(sizeof(permissions) < 1)
    {

#ifdef DEBUG

        write_file("/log/security", "Debug [security]: Using defalt permissions for '" + id + "' in '" + directory + "'.\n");

#endif
        permissions = data["(all)"];
    }


    if(sizeof(permissions) < 1 || !arrayp(permissions))
    {

#ifdef DEBUG

        write_file("/log/security", "Debug [security]: No permissions found for '" + id + "' in '" + directory + "'.\n");

#endif
        return 0;
    }

    switch(type)
    {
    //read, write, network, shadow, link, execute, bind, ownership
        case 1 : if(member_array("r", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted to read for " + directory + "\n");
#endif
            return 1;
        }
        break;
        case 2 : if(member_array("w", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted to write for " + directory + "\n");
#endif
            return 1;
        }
        break;
        case 3 : if(member_array("n", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted to access network for " + directory + "\n");
#endif
            return 1;
        }
        break;
        case 4 : if(member_array("s", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted to shadow for " + directory + "\n");
#endif
            return 1;
        }
        break;
        case 5 : if(member_array("l", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted to link for " + directory + "\n");
#endif
            return 1;
        }
        break;
        case 6 : if(member_array("e", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted to execute for " + directory + "\n");
#endif
            return 1;
        }
        break;
        case 7 : if(member_array("b", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted to bind for " + directory + "\n");
#endif
            return 1;
        }
        break;
        case 8 : if(member_array("o", permissions) != -1)
        {
#ifdef DEBUG
            write_file("/log/security", "Debug [query_access]: Permission granted as ownership for " + directory + "\n");
#endif
            return 1;
        }
        break;

    }
#ifdef DEBUG
    write_file("/log/security", "Debug [query_access]: Permission denied (" + type + ").\n");
#endif
    return 0;
}

string *track_member(string id, string directory)
{
    mapping data = access[directory];
    string *keys = keys(data);
    string *groupData = ({});
    int i;

#ifdef DEBUG

    write_file("/log/security", "Debug [security]: Tracking member '" + id + "' for '" + directory + "'.\n");

#endif

    for(i = 0; i < sizeof(keys); i++)
    {
        groupData = query_group(keys[i]);
        if(!arrayp(groupData) || sizeof(groupData) < 1) continue;
        if(member_array(id, groupData) != -1) return data[keys[i]];
    }

    return ({});
}

string *query_group(string group)
{
    if(!groups[group]) sscanf(group, "(%s)", group);
    return groups[group];
}

int isMember(string user, string group)
{
    string *data;
    int i;

    data = ({});

    if(!user || !group) return 0;

    data = groups[group];
    if(!arrayp(data)) return 0;

    for(i = 0; i < sizeof(data); i++)
    {
        string group_name;

        if(!stringp(data[i])) continue;

        if(sscanf(data[i], "(%s)", group_name))
        {
            data += groups[group_name];
        }
    }

    if(member_array(user, data) != -1) return 1;

    else return 0;
}

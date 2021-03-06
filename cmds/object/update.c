//update.c

//Byre @ LPUniversity
//05-APR-05
//Object management

//Last edited July 11th, 2006 by Tacitus


int main(string file)
{
     object obj;
     string error, *users;

     if(!file)
     {
          if(!this_player()->query("cwf")) return notify_fail("Error [update]: You must provide an argument. Syntax: update <file>\n");
          file = this_player()->query("cwf");
     }

     file=resolve_path(this_player()->query("cwd"), file);

     if(obj = find_object(file))
     {
         users = filter_array(all_inventory(obj), (: interactive :));
         users->move(load_object(VOID_OB), 1);
          destruct(obj);
     }

     if(file[<2..<1] != ".c") file += ".c";


     if(!file_exists(file)) return notify_fail("Error [update]: " + file  + " does not exist.\n");
     this_player()->set("cwf", file);
     error=catch(obj = load_object(file));

     if(error){
          write("Error [update]: Failed to load: "+file+"\nError: "+error+"\n");
          return 1;
     }

    if(arrayp(users)) users->move(obj, 1);
     write("Successful [update]: " +  file+" was updated.\n");
     return 1;
}

string help() {
     return (HIW + " SYNTAX: " + NOR + "update <file>\n\n" +
     "The update command loads the specified file. If no argument is given,\n"
     "then it will reloads the current working file. You can also specify here\n"
     "to update the enviroment you are in. Updating a file only affects the\n"
     "master copy of the object. Also note that clones of the object that were\n"
     "cloned before the update will NOT be affected. You will need to destroy\n"
     "Those objects and re-clone them for any changes to take effect.\n");
}

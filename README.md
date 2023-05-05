# CSS_Parser

Program starts in CSS reading mode, give it correct CSS on standard input and your CSS will be stored in memory as a List of Blocks, each Block contains array of Sections each Section contains a List of Selectors and a List of Attributes, each Attribute contains its name and value
in CSS mode you can view current state of CSS in program's memory by typing ` 

???? - changes mode to reading commands
NOTE for all commands with numbers: indexing starts with 1
NOTE for all commands: if no output is given either the command is wrong or there are no elements meeting criteria
Commands:
**** - changes mode to reading CSS
? – print number of CSS sections;
i,S,? – print number of Selectors for Section i;
i,A,? - print number of Attributes for Section i;
i,S,j – print Selector with number j in the Section i;
i,A,n – print Value of Attribute named n in the Section i;
n,A,? – print number of Attributes named n in all of the code (duplicates in each section are removed while reading)
z,S,? – ptint number of Sections containing Selector named z;
z,E,n – print Value of Attribute named n in Section with Selector named z (in case there are multiple Sections with Selector z only the last one counts);
i,D,* - delete Section i, if successful print "deleted";
i,D,n – delete Attribute named n from Section i, if successful print "deleted"


P.S. As you can probably see there is a Class named String, it is there because I wasn't allowed to use std::string, I know the code is garbage but it works so enjoy it I guess

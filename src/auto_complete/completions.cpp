//
// Created by swaraj on 6/28/25.
//
/*
 * create a tries tree
 * when user press tab copy m_command in different variable remove space and search that in tries tree
 * tries tree will contain
 * for cmmands like :
 *  - cd : need directory so only directory path with / completion should look something like cd abc
 *      search abc in current dir if presnet then abc/
 *          if not present then list out all direcotry
 *      output 1st line contain all flags of  cd ,and 2nd line conatine the directory
 *  - cat same as cd but for file
 * for other exutable like ls , echo etc we can not know what kind of output they take so
 *  1st line its flag
 *  2nd line current dir , and files
 * for cmmands like git docker we can add
 * to add command run --help with that cammnd try to extract the flags from it
 */
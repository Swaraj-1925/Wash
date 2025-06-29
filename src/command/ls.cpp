//
// Created by swaraj on 6/28/25.
//

#include "ls.h"

ThemeColor LsCommand::get_theme(std::string title) {
    Theme t;
    if (title == "permission") return t.PERMISSION;
    else if (title == "hyperlink") return t.LINK;
    else if (title == "user") return t.USER;
    else if (title == "group") return t.GROUP;
    else if (title == "time") return t.TIME;

    else if (title == "norm") return t.NORM_FILE;
    else if (title == "executable") return t.EXECUTABLE;
    else if (title == "symlink") return t.SYM_LINK;
    else if (title == "dotfile") return t.DOT_FILE;
    else if (title == "directory") return t.DIR;
    else if (title == "size") return t.SIZE;
    return t.NORM_FILE;
}

int LsCommand::render_output(ncpp::Plane *plane, int line) {
    int dim_x = 0;
    if (LsCommand::long_format) {
        for (const auto &it: files) {
            ThemeColor color = get_theme("permission");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 0, "%s", it.permissions.c_str());

            color = get_theme("hyperlink");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 12, "%d", it.num_hardlinks);

            color = get_theme("user");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 14, "%s", it.owner.c_str());

            color = get_theme("group");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 22, "%s", it.group.c_str());

            color = get_theme("size");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 31, "%s", it.size.c_str());

            color = get_theme("time");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 40, "%s %s %s", it.month.c_str(), it.day.c_str(), it.time_or_year.c_str());


            color = get_theme(it.type);
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 55, "%s", it.name.c_str());

            line++;
        }
    } else {
        unsigned dim_x = plane->get_dim_x();
        int max_x = (int)dim_x - 60;
        int cursor_x = 0;
        const int spacing = 2;

        for (const auto &it : files) {
            ThemeColor color = get_theme(it.type);
            plane->set_fg_rgb8(color.r(), color.g(), color.b());

            int name_len = it.name.length() + spacing;

            if (cursor_x + name_len >= max_x) {
                line++;
                cursor_x = 0;
            }

            plane->printf(line, cursor_x, "%s", it.name.c_str());
            cursor_x += name_len;
        }
    }
    files.clear();
    LsCommand::long_format = false;
    return line;
}

void LsCommand::parse_output(const std::vector<char *> &output, std::vector<char *> &exec_args) {
    for (char *arg: exec_args) {
        if (arg == nullptr) continue;
        std::string s(arg);
        if (s == "-la" || s == "-al" || s == "-l") {
            long_format = true;
            break;
        }
    }
    for (char *line: output) {
        if (!line || std::strlen(line) == 0) continue;
        std::string l(line);

        if (l.find("total ") == 0 || l.find("Resource temporarily unavailable") != std::string::npos)
            continue;

        std::istringstream iss(l);
        FileInfo fi;

        if (long_format) {
            iss >> fi.permissions
                >> fi.num_hardlinks
                >> fi.owner
                >> fi.group
                >> fi.size
                >> fi.month >> fi.day >> fi.time_or_year
                >> std::ws;
            std::getline(iss, fi.name);
            if (!fi.permissions.empty()) {
                if (fi.permissions[0] == 'd') fi.type = "directory";
                else if (fi.permissions[0] == 'l') fi.type = "symlink";
                else if (fi.name.size() > 0 && fi.name[0] == '.') fi.type = "dotfile";
                else if (fi.permissions[3] == 'x' || fi.permissions[6] == 'x' || fi.permissions[9] == 'x')
                    fi.type = "executable";
                else fi.type = "file";
            }
            files.push_back(fi);
        } else {
            std::getline(iss, fi.name);
            struct stat st;
            if (lstat(fi.name.c_str(), &st) == 0) {
                if (S_ISDIR(st.st_mode)) fi.type = "directory";
                else if (S_ISLNK(st.st_mode)) fi.type = "symlink";
                else if (fi.name[0] == '.') fi.type = "dotfile";
                else if (st.st_mode & S_IXUSR) fi.type = "executable";
                else fi.type = "file";
            } else {
                fi.type = "file";
            }
            files.push_back(fi);
        }
    }
    for (char* line : output) {
        free(line);  // necessary to avoid memory leaks
    }
}

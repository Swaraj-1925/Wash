//
// Created by swaraj on 6/21/25.
//
#include "command/ls.h"
LsCommand::LsCommand() = default;

Output LsCommand::execute(const std::vector<std::string> &args) {
    parse_args(args);
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    LsCommand::path = path.empty() ? cwd :path;
    try {
        for (const auto &entry: std::filesystem::directory_iterator(LsCommand::path)) {
            std::string name = entry.path().filename().string();
            if (!show_all && name[0] == '.') continue;

                FileInfo info;
                info.name = name;

                struct stat st;
                uintmax_t size;
                if (stat(entry.path().c_str(), &st) == 0) {
                    info.num_hardlinks = st.st_nlink;
                    size = st.st_size;
                    struct passwd* pw = getpwuid(st.st_uid);
                    struct group* gr = getgrgid(st.st_gid);
                    info.owner = pw ? pw->pw_name : std::to_string(st.st_uid);
                    info.group = gr ? gr->gr_name : std::to_string(st.st_gid);
                } else {
                    info.num_hardlinks= 0;
                    size = st.st_size;
                    info.owner = "unknown";
                    info.group = "unknown";
                }

                info.size = LsCommand::format_size(size);
                    //the filesystem time is in epochs we need to convert it into system time that is time we use
                    auto ftime = std::filesystem::last_write_time(entry);
                    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            //feacth current file time clock minus it with ftime and add system clock now to synchronization with system clock
                            ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now()
                    );
                    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
                    info.mtime = cftime;
                    auto perms = entry.status().permissions();
                    std::string perm_str;
                    if (entry.is_symlink()) {
                        perm_str += "l";
                    } else if (entry.is_directory()) {
                        perm_str += "d";
                    } else {
                        perm_str += "-";
                    }
                    LsCommand::get_long_format(perms, perm_str);
                    info.permissions = perm_str;
                    if (info.name[0] == '.') {
                        info.type = "dot";
                    } else if (entry.is_symlink()) {
                        info.type = "symlink";
                    } else if (entry.is_directory()) {
                        info.type = "dir";
                    } else if ((entry.status().permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) {
                        info.type = "executable";
                    } else {
                        info.type = "norm";
                    }
            LsCommand::files.push_back(info);
        }
    }catch (const std::filesystem::filesystem_error& e) {
        return Output{
                FAILURE_INTERNAL_ERROR,
                std::string("ls: filesystem error - ") + e.what()
        };
    }

    if(LsCommand::sort_time || LsCommand::sort_size || LsCommand::reverse){
        LsCommand::sort_files();
    }

return Output{.status_code = SUCCESS,
              .files_output= LsCommand::files
    };
}
void LsCommand::parse_args(const std::vector<std::string> &args) {
    LsCommand::show_all = false;
    LsCommand::long_format = false;
    LsCommand::human_readable = false;
    LsCommand::sort_size = false;
    LsCommand::sort_time = false;
    LsCommand::reverse = false;
    LsCommand::invalid = false;
    LsCommand::path.clear();
    LsCommand::files.clear();

    for (const auto& arg : args) {
        if (arg == "-a") LsCommand::show_all = true;
        else if (arg == "-l") LsCommand::long_format = true;
        else if (arg == "-h") LsCommand::human_readable = true;
        else if (arg == "-s") LsCommand::sort_size = true;
        else if (arg == "-t") LsCommand::sort_time = true;
        else if (arg == "-r") LsCommand::reverse = true;
        else if (!arg.empty() && arg[0] != '-') path = arg;
        else LsCommand::invalid = true;
    }
}
void LsCommand::get_long_format(std::filesystem::perms perms,std::string &perm_str){
    // user permission
    perm_str += (perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-";
    perm_str += (perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-";
    perm_str += (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-";

    // other user in same group as user
    perm_str += (perms & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-";
    perm_str += (perms & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-";
    perm_str += (perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-";

    // other groups user permission
    perm_str += (perms & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-";
    perm_str += (perms & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-";
    perm_str += (perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-";

}
void LsCommand::sort_files() {
    std::sort(LsCommand::files.begin(), LsCommand::files.end(), [&](const FileInfo& a, const FileInfo& b) {

        if (LsCommand::sort_size) return LsCommand::reverse ? a.size < b.size : a.size > b.size;

        else if (LsCommand::sort_time) return LsCommand::reverse ? a.mtime < b.mtime : a.mtime > b.mtime;

        return LsCommand::reverse ? a.name > b.name : a.name < b.name;
    });
}

std::string LsCommand::format_size(uintmax_t size) {
    std::ostringstream out;
    if (human_readable) {
        static const char* units[] = { "B", "K", "M", "G", "T" };
        int i = 0;
        double dsize = size;
        while (dsize >= 1024 && i < 4) {
            dsize /= 1024;
            ++i;
        }
        out << std::fixed << std::setprecision(1) << dsize << units[i];
    } else {
        out << size;
    }
    return out.str();
}

ThemeColor LsCommand::get_theme(std::string title) {
    Theme t;
    if(title == "permission") return t.PEACH;
    else if(title == "hyperlink") return t.TEAL;
    else if(title == "user") return t.YELLOW;
    else if(title == "group") return t.GREEN;
    else if(title == "time") return t.MAROON;
    else if (title == "norm") return t.TEXT0;
    else if (title == "executable") return t.SAPPHIRE;
    else if (title == "symlink") return t.BLUE;
    else if (title == "dot") return t.SUB_TEXT0;
    else if (title == "size") return t.TEXT0;
    return t.TEXT0;
}

int LsCommand::render_output(ncpp::Plane *plane, Output output,int line) {
    std::vector<FileInfo> result = output.files_output;
    int dim_x = 0;
    if(LsCommand::long_format){
        for (const auto &it: result){
            ThemeColor color = get_theme("permission");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line,0, "%s", it.permissions.c_str());

            color = get_theme("hyperlink");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line,12, "%d", it.num_hardlinks);

            color = get_theme("user");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line,14, "%s", it.owner.c_str());

            color = get_theme("group");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line,22, "%s", it.group.c_str());

            color = get_theme("size");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line,31, "%s", it.size.c_str());

            std::time_t t = it.mtime;
            std::tm tm_buf;
            localtime_r(&t, &tm_buf);
            char timebuf[64];
            // Example format: "Jun 21 14:05" or change to "%Y-%m-%d %H:%M:%S"

            std::strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", &tm_buf);

            color = get_theme("time");
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line, 40, "%s", timebuf);


            color = get_theme(it.type);
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(line,55, "%s", it.name.c_str());
            line ++;
        }
    }else{
        for (const auto &it: result){
            ThemeColor color = get_theme(it.type);
            plane->set_fg_rgb8(color.r(), color.g(), color.b());
            plane->printf(++line,NCALIGN_LEFT, "%s", it.name.c_str());
        }
    }

    return result.size() +1;
}
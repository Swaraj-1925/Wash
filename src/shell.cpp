//
// Created by swaraj on 6/4/25.
//
#include <ncpp/NotCurses.hh>
#include <iostream>
int shell(ncpp::Plane *std_plane,ncpp::NotCurses &nc){
    ncinput ni;
    unsigned dim_y,dim_x;

    nc.get_term_dim(&dim_y,&dim_x);
    std_plane->printf((int )dim_y-1,ncpp::NCAlign::Left,"y:%03u , x:%03u" , dim_y,dim_x);
    nc.render();

    while(nc.get(false, &ni) != (char32_t)-1){
        std_plane->printf(0,ncpp::NCAlign::Left,"inside the loop");
        nc.render();
    }
    return EXIT_SUCCESS;
}
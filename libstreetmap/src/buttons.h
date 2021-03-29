//
// Created by ahmadda4 on 2021-03-28.
//

#ifndef MAPPER_BUTTONS_H
#define MAPPER_BUTTONS_H

void selectButtonClk(GtkEntry *,ezgl::application *application); //widgit to hold btn for select map

void colorBlindToggle(GtkEntry *,ezgl::application *application);

bool checkPOIFilter(std::__cxx11::string);

void docButtonClk(GtkEntry*, ezgl::application*);

void bkButtonClk(GtkEntry*, ezgl::application*);

void carButtonClk(GtkEntry*, ezgl::application*);

void artButtonClk(GtkEntry*, ezgl::application*);

void foodButtonClk(GtkEntry*, ezgl::application*);

void loadFilterButtons(ezgl::application*);

void filClrClk(GtkEntry*, ezgl::application*);

void monButtonClk(GtkEntry*, ezgl::application*);

#endif //MAPPER_BUTTONS_H

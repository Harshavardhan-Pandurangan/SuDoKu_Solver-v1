#include<gtk/gtk.h>
#include<pango/pango.h>
#include<string.h>
#include<stdio.h>

static GtkWidget *sudoku_grid_values[9][9];         //object buttons for the sudoku grid
static int sudoku_grid_values_pencil[9][9][9];      //list of 'penciled' numbers for each cell of grid
static int sudoku_grid_numbers[9][9];               //grid of final entries 
static int sudoku_ques_grid[9][9];                  //grid to know the ques cells and answer cells
static GtkWidget *pencil_pen_numbers[9];            //object buttons for the entry grid

static GtkWidget *main_window;
static GtkWidget *manual_solve;
static GtkWidget *pencil_view;
static GtkWidget *pen, *pencil, *auto_solve;
static GtkWidget *dialog;
static int switch_count_grid = 0;           //to avoid recursion during sudoku grid cell switching
static int switch_count_pen_pencil = 0;     //to avoid recursion during pen pencil switching
static int number_grid_to_click_update = 0; //to avoid recursion during click_update transfer from grid
static int click_update_to_number_grid = 0; //to avoid recursion during grid transfer from click_update
static int ques_set_check = 0;              //controls unwanted functions while setting question
static int ques_clear = 0;                  //to erase ques cells
static int switch_pen_pencil = 1;           //pen->1 , pencil->0
static int a = 4, b = 4;                    //selected sudoku grid cell

//////////////////////////////////////////////////////////////////////////////////////////////////////

void click_update(int click_source){
    //click_source: 0-> pen_pencil, 1-> grid_clicked, 2-> number_grid_click, 3-> erase_cell
    //4-> ques set, 5-> auto_solve
    if(click_source==0){            //0-> pen_pencil
        if(switch_pen_pencil==0){
            if(sudoku_grid_numbers[a][b]==0){
                for(int i = 0 ; i < 9 ; i++){
                    gboolean val = (gboolean)sudoku_grid_values_pencil[a][b][i];
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), val);
                }
            }
        }else{
            for(int i = 0 ; i < 9 ; i++){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
            }
            if(sudoku_grid_numbers[a][b]!=0){
                int val = (sudoku_grid_numbers[a][b])-1;
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[val]), TRUE);
            }
        }
    }else if(click_source==1){      //1-> grid_clicked
        if(sudoku_grid_numbers[a][b]==0){
            if(switch_pen_pencil==0){
                for(int i = 0 ; i < 9 ; i++){
                    gboolean val = (gboolean)sudoku_grid_values_pencil[a][b][i];
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), val);
                }
            }
            if(switch_pen_pencil==1){
                click_update_to_number_grid = 1;
                for(int i = 0 ; i < 9 ; i++){
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
                }
                click_update_to_number_grid = 0;
            }
        }else{
            click_update_to_number_grid = 1;
            for(int i = 0 ; i < 9 ; i++){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
            }
            int val = sudoku_grid_numbers[a][b]-1;
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[val]), TRUE);
            click_update_to_number_grid = 0;
        }
    }else if(click_source==2){      //2-> number_grid_clicked
        if(switch_pen_pencil==0 && click_update_to_number_grid==0){
            for(int i = 0 ; i < 9 ; i++){
                if(sudoku_grid_values_pencil[a][b][i]==0){
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
                }else{
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), TRUE);
                }
            }
            char c[50];
            strncpy(c, "", 50);
            for(int i = 0 ; i < 9 ; i++){
                if(sudoku_grid_values_pencil[a][b][i]==1){
                    char d[2];
                    sprintf(d, "%d", i+1);
                    strcat(c, d);
                }else{
                    char d[3];
                    d[0]=(char)32;
                    d[1]=(char)32;
                    strcat(c, d);
                }

                if(i==0 || i==1 || i==3 || i==4 || i==6 || i==7){
                    char d[3];
                    d[0]=(char)32;
                    d[1]=(char)32;
                    strcat(c, d);
                }else if(i!=8){
                    char d[2];
                    d[0]=(char)10;
                    strcat(c, d);
                }
            }
            GtkWidget *new_label;
            GList *children, *iterator;
            new_label = gtk_label_new(c);
            PangoAttrList *Attrs = pango_attr_list_new();
            PangoAttribute *SizeAttr = pango_attr_size_new(6500);
            pango_attr_list_insert(Attrs, SizeAttr);
            gtk_label_set_attributes(GTK_LABEL(new_label), Attrs);
            pango_attr_list_unref(Attrs);
            children = gtk_container_get_children(GTK_CONTAINER(sudoku_grid_values[a][b]));
            for(iterator = children ; iterator!=NULL ; iterator = g_list_next(iterator)){
                gtk_widget_destroy(GTK_WIDGET(iterator->data));
            }
            gtk_container_add(GTK_CONTAINER(sudoku_grid_values[a][b]), GTK_WIDGET(new_label));
            gtk_widget_show(GTK_WIDGET(new_label));
        }else if((switch_pen_pencil==1) && click_update_to_number_grid==0){
            for(int i = 0 ; i < 9 ; i++){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
            }
            if(sudoku_grid_numbers[a][b]!=0){
                int val = sudoku_grid_numbers[a][b]-1;
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[val]), TRUE);
            }
            GList *children, *iterator;
            children = gtk_container_get_children(GTK_CONTAINER(sudoku_grid_values[a][b]));
            for(iterator = children ; iterator!=NULL ; iterator = g_list_next(iterator)){
                gtk_widget_destroy(GTK_WIDGET(iterator->data));
            }
            char c[2];
            if(sudoku_grid_numbers[a][b]!=0){
                sprintf(c, "%d", sudoku_grid_numbers[a][b]);
            }else{
                strncpy(c, "", 2);
            }
            gtk_button_set_label(GTK_BUTTON(sudoku_grid_values[a][b]), c);
        }
    }else if(click_source==3){      //3-> erase_cell
        ques_clear = 1;
        for(int i = 0 ; i < 9 ; i++){
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
        }
        ques_clear = 0;
    }else if(click_source==4){      //4-> ques_set
        if(ques_clear==0){
            for(int i = 0 ; i < 9 ; i++){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
            }
            int val = sudoku_grid_numbers[a][b]-1;
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[val]), TRUE);
            char c[2];
            sprintf(c, "%d", val+1);
            GtkWidget *new_label;
            GList *children, *iterator;
            new_label = gtk_label_new(c);
            gtk_widget_set_name(GTK_WIDGET(new_label), "myWhite_Button");
            children = gtk_container_get_children(GTK_CONTAINER(sudoku_grid_values[a][b]));
            for(iterator = children ; iterator!=NULL ; iterator = g_list_next(iterator)){
                gtk_widget_destroy(GTK_WIDGET(iterator->data));
            }
            gtk_container_add(GTK_CONTAINER(sudoku_grid_values[a][b]), GTK_WIDGET(new_label));
            gtk_widget_set_halign(GTK_WIDGET(new_label), GTK_ALIGN_FILL);
            gtk_widget_set_valign(GTK_WIDGET(new_label), GTK_ALIGN_FILL);
            gtk_widget_show(GTK_WIDGET(new_label));
        }else{
            gtk_button_set_label(GTK_BUTTON(sudoku_grid_values[a][b]), " ");
        }
    }else if(click_source==5){      //5-> auto_solve
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                int val = sudoku_grid_numbers[i][j];
                char c[2];
                sprintf(c, "%d", val);
                gtk_button_set_label(GTK_BUTTON(sudoku_grid_values[i][j]), c);
            }
        }
    }
}

void number_grid_click(GtkWidget *cell){
    //1-> manual and ques=0
    //2-> manual and ques=1
    //3-> auto and pencil
    //4-> auto and pen
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(manual_solve)) && sudoku_ques_grid[a][b]==0){
        if(number_grid_to_click_update==0){
            number_grid_to_click_update = 1;
            if(switch_pen_pencil==0){
                if(click_update_to_number_grid==0){
                    sudoku_grid_numbers[a][b] = 0;
                    for(int i = 0 ; i < 9 ; i++){
                        if(cell==pencil_pen_numbers[i]){
                            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cell))){
                                sudoku_grid_values_pencil[a][b][i] = 1;
                            }else{
                                sudoku_grid_values_pencil[a][b][i] = 0;
                            }
                        }
                    }
                }
            }else{
                if(click_update_to_number_grid==0){
                    for(int i = 0 ; i < 9 ; i++){
                        if(cell==pencil_pen_numbers[i] && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cell))){
                            sudoku_grid_numbers[a][b] = i+1;
                        }
                    }
                    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cell))){
                        for(int i = 0 ; i < 9 ; i++){
                            sudoku_grid_values_pencil[a][b][i] = 0;
                        }
                    }
                }
            }
            click_update(2);
            number_grid_to_click_update = 0;
        }
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(manual_solve)) && sudoku_ques_grid[a][b]==1){
        // Ignore. Not allowed to change question during solving.
    }
    if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(manual_solve)) && switch_pen_pencil==0){
        // Ignore. You don't need pencil while setting question.
    }
    if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(manual_solve)) && switch_pen_pencil==1){
        if(number_grid_to_click_update==0 && ques_set_check==0){
            number_grid_to_click_update = 1;
            for(int i = 0 ; i < 9 ; i++){
                sudoku_grid_values_pencil[a][b][i]==0;
            }
            if(ques_clear==0){
                for(int i = 0 ; i < 9 ; i++){
                    if(cell==pencil_pen_numbers[i]){
                        sudoku_grid_numbers[a][b] = i+1;
                        sudoku_ques_grid[a][b] = 1;
                    }
                }
            }
            click_update(4);
            number_grid_to_click_update = 0;
        }
    }
}

void grid_clicked(GtkWidget *cell){
    if(switch_count_grid==0){
        switch_count_grid=1;
        for(int i = 0 ; i < 9 ; i++){
            for(int j = 0 ; j < 9 ; j++){
                if(cell==sudoku_grid_values[i][j]){
                    a=i;
                    b=j;
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sudoku_grid_values[a][b]), TRUE);
                }else{
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sudoku_grid_values[i][j]), FALSE);
                }
            }
        }
        switch_count_grid=0;
    }
    ques_set_check = 1;
    click_update(1);
    ques_set_check = 0;
}

void solve_check_f2(){
    //When auto_solve switch is OFF, Check if the sudoku is solved
    //When auto_solve switch is ON, Solve the sudoku
    if(!gtk_switch_get_active(GTK_SWITCH(auto_solve))){         //check the solution
        int unique_elements(int temp_array[9]){
            int exists_check = 0;
            for(int i = 0 ; i < 8 ; i++){
                for(int j = i+1 ; j < i ; j++){
                    if(temp_array[i]==temp_array[j]){
                        exists_check = 1;
                    }
                }
            }
            if(exists_check==0){
                return TRUE;
            }else{
                return FALSE;
            }
        }

        int check_count = 0;
        for(int i = 0 ; i < 9 ; i++){
            int temp_row_array[9] = {};
            int temp_column_array[9] = {};
            for(int j = 0 ; j < 9 ; j++){
                temp_column_array[j] = sudoku_grid_numbers[i][j];
                temp_row_array[j] = sudoku_grid_numbers[j][i];
            }
            if(unique_elements(temp_column_array)){
                check_count+=1;
            }

            if(unique_elements(temp_row_array)){
                check_count+=1;
            }
        }
        
        if(check_count==18){
            for(int i = 0 ; i < 9 ; i++){
                int temp_array[9]={};
                int x = i*3;
                int y = x/9;    //row
                int z = x%9;    //column
                for(int j = 0 ; j < 3 ; j++){
                    for(int k = 0 ; k < 3 ; k++){
                        int m = y+j;
                        int n = z+k;
                        temp_array[(j*3)+k] = sudoku_grid_numbers[m][n];
                        
                    }
                }
                if(unique_elements(temp_array)){
                    check_count+=1;
                }
            }
        }

        if(check_count==27){
            printf("The sudoku is solved correctly");
        }else{
            printf("The sudoku is not solved correctly");
        }
    }else{                                                      //solve the puzzle
        //Prority based order to be followed each iteration
        //1-> pencil all, when allowed                  done
        //2-> check for 'pen'able elements and assign   done
        //3-> check for non-assignable rows or column   
        //4-> find existence of X-wing cases            
        
        int sudoku[9][9];
        for(int i = 0 ; i < 9 ; i++){
            for(int j = 0 ; j < 9 ; j++){
                if(sudoku_ques_grid[i][j]==1){
                    sudoku[i][j] = sudoku_grid_numbers[i][j];
                }else{
                    sudoku[i][j] = 0;
                }
            }
        }

        int pencil_all = 0;
        int not_solved = 1;
        int no_solution = 0;
        
        while(not_solved){
            if(pencil_all==0){
                pencil_all = 1;
                for(int i = 0 ; i < 9 ; i++){
                    for(int j = 0 ; j < 9 ; j++){
                        if(sudoku[i][j]!=0){
                            for(int k = 0 ; k < 9 ; k++){
                                sudoku_grid_values_pencil[i][j][k] = 1;
                            }
                            int array[9]={1,2,3,4,5,6,7,8,9};
                            for(int k = 0 ; i < 9 ; i++){
                                if(sudoku[i][k]!=0){
                                    sudoku_grid_values_pencil[i][j][sudoku[i][k]-1] = 0;
                                }

                                if(sudoku[k][j]!=0){
                                    sudoku_grid_values_pencil[i][j][sudoku[i][k]-1] = 0;
                                }
                            }
                        }
                    }
                }
            }

            for(int i = 0 ; i < 9 ; i++){
                for(int j = 0 ; j < 9 ; j++){
                    if(sudoku[i][j]==0){
                        int check = 0;
                        for(int k = 0 ; k < 9 ; k++){
                            if(sudoku_grid_values_pencil[i][j][k]==1){
                                check+=1;
                            }
                        }
                        if(check==1){
                            for(int k = 0 ; k < 9 ; k++){
                                if(sudoku_grid_values_pencil[i][j][k]==1){
                                    sudoku[i][j]=k+1;
                                }
                                sudoku_grid_values_pencil[i][j][k] = 0;
                            }
                        }
                    }
                }
            }

            //paused

            not_solved = 0;
            for(int i = 0 ; i < 9 ; i++){
                for(int j = 0 ; j < 9 ; j++){
                    if(sudoku[i][j]==0){
                        not_solved=1;
                    }
                }
            }

            for(int i = 0 ; i < 9 ; i++){
                for (int j = 0; j < 9 ; j++){
                    if(sudoku[i][j]==0){
                        int possibility_count = 0;
                        for (int k = 0; k < 9 ; k++){
                            if(sudoku_grid_values_pencil[i][j][k]==0){
                                possibility_count+=1;
                            }
                        }
                        if(possibility_count==9){
                            no_solution = 1;
                            not_solved = 0;
                        }
                    }
                }
            }
        }

        if(no_solution==1){
            printf("No solution");
        }
    }
}

void solve_check_f1(){
    //When auto_solve switch is OFF, check if all elements are filled and only then call next function
    //When auto_solve switch is ON, just call the next function
    if(!gtk_switch_get_active(GTK_SWITCH(auto_solve))){
        int check = 0;
        for(int i = 0 ; i < 9 ; i++){
            for(int j = 0 ; j < 9 ; j++){
                if(sudoku_grid_numbers[i][j]==0){
                    check = 1;
                }
            }
        }

        if(check==0){
            solve_check_f2();
        }
    }else{
        solve_check_f2();
    }
}

void manual_solve_set(){
    //sudoku set to be solved manually.
}

void pen_pencil(GtkWidget *cell){
    if(switch_count_pen_pencil==0){
        switch_count_pen_pencil=1;
        if(cell==pencil){
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pen), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil), TRUE);
            switch_pen_pencil=0;
        }else if(cell==pen){
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pen), TRUE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil), FALSE);
            switch_pen_pencil=1;
        }
    }
    switch_count_pen_pencil=0;
    click_update(0);
}

void erase_cell_f(){
    for(int i = 0 ; i < 9 ; i++){
        sudoku_grid_values_pencil[a][b][i]=0;
    }
    if((gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(manual_solve))) && sudoku_ques_grid[a][b]==0){
        sudoku_ques_grid[a][b] = 0;
        sudoku_grid_numbers[a][b] = 0;
    }else if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(manual_solve))){
        sudoku_ques_grid[a][b] = 0;
        sudoku_grid_numbers[a][b] = 0;
    }
    click_update(3);
}

void clear_sudoku_f2(){
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            sudoku_grid_numbers[i][j] = 0;
            sudoku_ques_grid[i][j] = 0;
            gtk_button_set_label(GTK_BUTTON(sudoku_grid_values[i][j]), " ");
        }
    }
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            for(int k = 0 ; k < 9 ; k++){
                sudoku_grid_values_pencil[i][j][k]=0;
            }
        }
    }
    for(int i = 0 ; i < 9 ; i++){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pencil_pen_numbers[i]), FALSE);
    }
    gtk_button_set_label(GTK_BUTTON(sudoku_grid_values[a][b]), " ");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pen), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sudoku_grid_values[4][4]), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(manual_solve), FALSE);
}

void clear_sudoku_f1(){
    GtkWidget *ques_label;
    GtkWidget *content_area;
    GtkWidget *yes_button, *cancel_button;

    dialog = gtk_dialog_new_with_buttons("Clear Sudoku?", GTK_WINDOW(main_window),
    GTK_DIALOG_DESTROY_WITH_PARENT, "Cancel", 1, "Yes", 2, NULL);
    
    gtk_widget_set_size_request(GTK_WIDGET(dialog), 300, 100);    
    ques_label = gtk_label_new("\nAre you sure to clear the sudoku?\nThis change cannot be undone");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area), GTK_WIDGET(ques_label));
    gtk_widget_show_all(GTK_WIDGET(dialog));
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if(response==2){
        clear_sudoku_f2();
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void myCSS(){
    GtkCssProvider *css_provider;
    GdkDisplay *display;
    GdkScreen *screen;
    
    css_provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(GDK_SCREEN(screen), GTK_STYLE_PROVIDER(css_provider),
    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *My_CSS_File = "mystyle.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(GTK_CSS_PROVIDER(css_provider),
    g_file_new_for_path(My_CSS_File), &error);
    g_object_unref(GTK_CSS_PROVIDER(css_provider));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            sudoku_grid_numbers[i][j]=0;
            sudoku_ques_grid[i][j]=0;
        }
    }

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            for(int k = 0 ; k < 9 ; k++){
                sudoku_grid_values_pencil[i][j][k]=0;
            }
        }
    }

    GtkWidget *main_screen_container;
    GtkWidget *sudoku_grid, *functions_grid;
    GtkWidget *grid[9];
    GtkWidget *sep1, *sep2, *sep3, *sep4, *sep5, *sep6;
    GtkWidget *entering_numbers_grid;
    GtkWidget *auto_solve_label, *solve_check, *erase_cell, *clear_sudoku;

    gtk_init(&argc, &argv);
    myCSS();

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "SuDoKu Solver");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 375);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    gtk_window_set_gravity(GTK_WINDOW(main_window), GTK_ALIGN_CENTER);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_set_visible(GTK_WIDGET(main_window), TRUE);

    main_screen_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(main_screen_container));
    gtk_widget_set_visible(GTK_WIDGET(main_screen_container), TRUE);

    sudoku_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(sudoku_grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(sudoku_grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(sudoku_grid), 6);
    gtk_grid_set_row_spacing(GTK_GRID(sudoku_grid), 6);

    for(int i = 0 ; i < 9 ; i++){
        grid[i] = gtk_grid_new();
        gtk_grid_set_row_homogeneous(GTK_GRID(grid[i]), TRUE);
        gtk_grid_set_column_homogeneous(GTK_GRID(grid[i]), TRUE);
        gtk_grid_set_row_spacing(GTK_GRID(grid[i]), 1);
        gtk_grid_set_column_spacing(GTK_GRID(grid[i]), 1);
        gtk_widget_set_size_request(GTK_WIDGET(grid[i]), 120, 120);
        gtk_grid_attach(GTK_GRID(sudoku_grid), grid[i],i%3,i/3,1,1);
    }

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            int a=i/3;
            int b=j/3;

            int grid_num=a+(3*b);
            sudoku_grid_values[i][j] = gtk_toggle_button_new();
            gtk_button_set_label(GTK_BUTTON(sudoku_grid_values[i][j]), " ");

            gtk_grid_attach(GTK_GRID(grid[grid_num]),
            GTK_WIDGET(sudoku_grid_values[i][j]), i%3,j%3,1,1);
        }
    }
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sudoku_grid_values[4][4]), TRUE);
    gtk_widget_set_halign(GTK_WIDGET(sudoku_grid_values[4][4]), GTK_ALIGN_FILL);
    gtk_widget_set_valign(GTK_WIDGET(sudoku_grid_values[4][4]), GTK_ALIGN_FILL);
    gtk_widget_set_size_request(GTK_WIDGET(sudoku_grid_values[4][4]), 59, 40);

    functions_grid = gtk_grid_new();

    sep1 = gtk_label_new("         ");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(sep1), 0, 0, 1, 3);
    sep2 = gtk_label_new("         ");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(sep2), 2, 0, 1, 3);
    sep3 = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(sep3), 4, 3, 1, 1);
    sep4 = gtk_label_new("\n");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(sep4), 4, 5, 1, 1);
    sep5 = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(sep5), 4, 7, 1, 1);
    sep6 = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(sep6), 4, 9, 1, 1);

    auto_solve_label = gtk_label_new("Auto Solve");
    gtk_widget_set_halign(GTK_WIDGET(auto_solve_label), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(auto_solve_label), GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(auto_solve_label), 1, 0, 1, 1);

    auto_solve = gtk_switch_new();
    gtk_widget_set_halign(GTK_WIDGET(auto_solve), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(auto_solve), GTK_ALIGN_START);
    gtk_widget_set_margin_top(GTK_WIDGET(auto_solve), 2);
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(auto_solve), 1, 1, 1, 1);

    solve_check = gtk_button_new_with_label("          SOLVE /\nCHECK ANSWER");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(solve_check), 1, 2, 1, 1);
    gtk_widget_set_halign(GTK_WIDGET(solve_check), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(solve_check), GTK_ALIGN_CENTER);

    entering_numbers_grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(entering_numbers_grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(entering_numbers_grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(entering_numbers_grid), 1);
    gtk_grid_set_column_spacing(GTK_GRID(entering_numbers_grid), 1);
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(entering_numbers_grid), 3, 0, 3, 3);
    gtk_widget_set_size_request(GTK_WIDGET(entering_numbers_grid), 120, 120);

    for(int i = 0 ; i < 9 ; i++){
        char c[2];
        sprintf(c, "%d", i+1);
        pencil_pen_numbers[i] = gtk_toggle_button_new_with_label(c);
        gtk_grid_attach(GTK_GRID(entering_numbers_grid), GTK_WIDGET(pencil_pen_numbers[i]),
        i%3, i/3, 1, 1);
    }

    manual_solve = gtk_toggle_button_new_with_label("Solve Manual");
    gtk_widget_set_halign(GTK_WIDGET(manual_solve), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(manual_solve), GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(manual_solve), 1, 4, 1, 1);

    pencil_view = gtk_text_view_new();
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(pencil_view), TRUE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(pencil_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(pencil_view), FALSE);
    gtk_text_view_set_justification(GTK_TEXT_VIEW(pencil_view), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(pencil_view), 3, 4, 3, 1);

    pencil = gtk_toggle_button_new_with_label("Pencil");
    gtk_widget_set_halign(GTK_WIDGET(pencil), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(pencil), GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(GTK_WIDGET(pencil), 90, -1);
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(pencil), 1, 6, 1, 1);

    pen = gtk_toggle_button_new_with_label("Pen");
    gtk_widget_set_halign(GTK_WIDGET(pen), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(pen), GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(GTK_WIDGET(pen), 90, -1);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pen), TRUE);
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(pen), 1, 8, 1, 1);

    erase_cell = gtk_button_new_with_label("Erase Cell");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(erase_cell), 4, 6, 1, 1);

    clear_sudoku = gtk_button_new_with_label("Clear Sudoku");
    gtk_grid_attach(GTK_GRID(functions_grid), GTK_WIDGET(clear_sudoku), 4, 8, 1, 1);

    gtk_box_pack_start(GTK_BOX(main_screen_container), sudoku_grid, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_screen_container), functions_grid, FALSE, TRUE, 0);

    g_signal_connect(solve_check, "clicked", G_CALLBACK(solve_check_f1), NULL);
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            g_signal_connect(sudoku_grid_values[i][j],"clicked",
            G_CALLBACK(grid_clicked), sudoku_grid_values[i][j]);
        }
    }
    for(int i = 0 ; i < 9 ; i++){
        g_signal_connect(pencil_pen_numbers[i], "clicked",
        G_CALLBACK(number_grid_click), pencil_pen_numbers[i]);
    }
    g_signal_connect(manual_solve, "clicked", G_CALLBACK(manual_solve_set), NULL);
    g_signal_connect(pencil, "clicked", G_CALLBACK(pen_pencil), pencil);
    g_signal_connect(pen, "clicked", G_CALLBACK(pen_pencil), pen);
    g_signal_connect(erase_cell, "clicked", G_CALLBACK(erase_cell_f), NULL);
    g_signal_connect(clear_sudoku, "clicked", G_CALLBACK(clear_sudoku_f1), NULL);

    gtk_widget_show_all(main_window);
    gtk_main();

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

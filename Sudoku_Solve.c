#include<stdio.h>

int main(){
    int sudoku_grid_values_pencil[9][9][9];
    int sudoku[9][9];

    int a;
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            scanf("%d", &a);
            sudoku[i][j] = a;
        }
    }

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            printf("%3d", sudoku[i][j]);
        }
        printf("\n");
    }

    int pencil_all = 0;
    int not_solved = 1;
    int no_solution = 0;
    
    while(not_solved){

        //Initial pencil marking(one time exercise)
        //Setting all values of pencil for each cell as true and then removing pencil
        //values wrt rows, columns and 3x3 grids.
        if(pencil_all==0){
            pencil_all = 1;
            for(int i = 0 ; i < 9 ; i++){
                for(int j = 0 ; j < 9 ; j++){
                    for(int k = 0 ; k < 9 ; k++){
                        sudoku_grid_values_pencil[i][j][k] = 1;
                    }
                    if(sudoku[i][j]==0){
                        int array[9]={1,2,3,4,5,6,7,8,9};
                        for(int k = 0 ; k < 9 ; k++){
                            if(sudoku[i][k]!=0){
                                sudoku_grid_values_pencil[i][j][sudoku[i][k]-1] = 0;    //Check row
                            }

                            if(sudoku[k][j]!=0){
                                sudoku_grid_values_pencil[i][j][sudoku[k][j]-1] = 0;    //check column
                            }
                        }

                        int x= i/3;
                        int y= j/3;
                        for(int p = 0 ; p < 3 ; p++){
                            for(int q = 0 ; q < 3 ; q++){
                                int temp = sudoku[(x*3)+p][(y*3)+q];
                                if(temp!=0){
                                    sudoku_grid_values_pencil[i][j][temp-1] = 0;
                                }
                            }
                        }
                    }else{
                        for(int l = 0 ; l < 9 ; l++){
                            sudoku_grid_values_pencil[i][j][l]=0;
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
                                sudoku_grid_values_pencil[i][j][k] = 0;
                            }
                        }

                        for(int k = 0 ; k < 9 ; k++){
                            sudoku_grid_values_pencil[i][k][sudoku[i][j]-1] = 0;
                            sudoku_grid_values_pencil[k][j][sudoku[i][j]-1] = 0;
                        }

                        int x = i/3;
                        int y = j/3;
                        for(int p = 0 ; p < 3 ; p++){
                            for(int q = 0 ; q < 3 ; q++){
                                sudoku_grid_values_pencil[(x*3)+p][(y*3)+q][sudoku[i][j]-1] = 0;
                            }
                        }
                    }
                }
            }
        }

        for(int k = 0 ; k < 9 ; k++){
            for(int i = 0 ; i < 9 ; i++){
                int count = 0, check = 0, row = -1, column = -1;
                for(int p = 0 ; p < 9 ; p++){
                    if(sudoku_grid_values_pencil[i][p][k]==1){
                        count+=1;
                        row = i;
                        column = p;
                    }

                    if(sudoku[i][p]==(k+1)){
                        check+=1;
                    }
                }
                if(check==0 && count==1){
                    for(int q = 0 ; q < 9 ; q++){
                        sudoku_grid_values_pencil[row][column][q] = 0;
                    }
                    sudoku[row][column] = k+1;
                }
            }

            for(int j = 0 ; j < 9 ; j++){
                int count = 0, check = 0, row = -1, column = -1;
                for(int p = 0 ; p < 9 ; p++){
                    if(sudoku_grid_values_pencil[p][j][k]==1){
                        count+=1;
                        row = p;
                        column = j;
                    }

                    if(sudoku[p][j]==(k+1)){
                        check+=1;
                    }
                }
                if(check==0 && count==1){
                    for(int q = 0 ; q < 9 ; q++){
                        sudoku_grid_values_pencil[row][column][q] = 0;
                    }
                    sudoku[row][column] = k+1;
                }
            }
        }

        for(int x = 0 ; x < 3 ; x++){
            for(int y = 0 ; y < 3 ; y++){
                for(int k = 0 ; k < 9 ; k++){
                    int count = 0;
                    for(int p = 0 ; p < 3 ; p++){
                        for(int q = 0 ; q < 3 ; q++){
                            if(sudoku_grid_values_pencil[(x*3)+p][(y*3)+q][k]==1){
                                count+=1;
                            }
                        }
                    }
                    if(count==1){
                        int i,j;
                        for(int p = 0 ; p < 3 ; p++){
                            for(int q = 0 ; q < 3 ; q++){
                                if(sudoku_grid_values_pencil[(x*3)+p][(y*3)+q][k]==1){
                                    i = (x*3)+p;
                                    j = (y*3)+q;
                                }
                            }
                        }
                        sudoku[i][j]=k+1;

                        for(int l = 0 ; l < 9 ; l++){
                            sudoku_grid_values_pencil[i][j][l] = 0;
                            sudoku_grid_values_pencil[i][l][k] = 0;
                            sudoku_grid_values_pencil[l][j][k] = 0;
                        }

                        for(int p = 0 ; p < 3 ; p++){
                            for(int q = 0 ; q < 3 ; q++){
                                sudoku_grid_values_pencil[(x*3)+p][(y*3)+q][k]=0;
                            }
                        }
                    }
                }
            }
        }

        for(int k = 0 ; k < 9 ; k++){
            for(int x = 0 ; x < 3 ; x++){
                for(int y = 0 ; y < 3 ; y++){
                    for(int p = 0 ; p < 3 ; p++){
                        int count_row = 0;
                        int count_column = 0;
                        int j1, i2;
                        for(int q = 0 ; q < 3 ; q++){
                            int count_row_sub = 0;
                            int count_column_sub = 0;
                            for(int r = 0 ; r < 3 ; r++){
                                if(sudoku_grid_values_pencil[(p*3)+r][(y*3)+q][k]==1){
                                    count_row_sub+=1;
                                }

                                if(sudoku_grid_values_pencil[(x*3)+q][(p*3)+r][k]==1){
                                    count_column_sub+=1;
                                }
                            }

                            if(count_row_sub>0){
                                count_row+=1;
                                j1 = (y*3)+q;
                            }

                            if(count_column_sub>0){
                                count_column+=1;
                                i2 = (x*3)+q;
                            }
                        }

                        if((count_row==1) && (p!=x)){
                            for(int r = 0 ; r < 3 ; r++){
                                sudoku_grid_values_pencil[(x*3)+r][j1][k] = 0;
                            }
                        }

                        if((count_column==1) && (p!=y)){
                            for(int r = 0 ; r < 3 ; r++){
                                sudoku_grid_values_pencil[i2][(y*3)+r][k] = 0;
                            }
                        }
                    }
                }
            }
        }

        for(int k = 0 ; k < 9 ; k++){
            for(int x = 0 ; x < 3 ; x++){
                for(int y = 0 ; y < 3 ; y++){
                    int count_row = 0;
                    int count_column = 0;
                    int i[4] = {-1,-1,-1,-1}, j[4] = {-1,-1,-1,-1}, t1[2] = {-1,-1}, t2[2] = {-1,-1};
                    for(int p = 0 ; p < 3 ; p++){
                        for(int q = 0 ; q < 3 ; q++){
                            int count_row_sub = 0;
                            int count_column_sub = 0;
                            for(int r = 0 ; r < 3 ; r++){
                                if(sudoku_grid_values_pencil[(p*3)+r][(y*3)+q][k]==1){
                                    count_row_sub+=1;
                                }

                                if(sudoku_grid_values_pencil[(x*3)+q][(p*3)+r][k]==1){
                                    count_column_sub+=1;
                                }
                            }

                            if(count_row_sub>0){
                                count_row+=1;
                                if(t1[0]==-1){
                                    t1[0] = p;
                                }else if(t1[0]!=p && t1[1]==-1){
                                    t1[1] = p;
                                }

                                if(i[0]==-1){
                                    i[0] = (y*3)+q;
                                }else if(i[1]==-1){
                                    i[1] = (y*3)+q;
                                }else if(i[2]==-1){
                                    i[2] = (y*3)+q;
                                }else if(i[3]==-1){
                                    i[3] = (y*3)+q;
                                }
                            }

                            if(count_column_sub>0){
                                count_column+=1;
                                if(t2[0]==-1){
                                    t2[0] = p;
                                }else if(t2[0]!=p && t2[1]==-1){
                                    t2[1] = p;
                                }

                                if(j[0]==-1){
                                    j[0] = (x*3)+q;
                                }else if(j[1]==-1){
                                    j[1] = (x*3)+q;
                                }else if(j[2]==-1){
                                    j[2] = (x*3)+q;
                                }else if(j[3]==-1){
                                    j[3] = (x*3)+q;
                                }
                            }
                        }
                    }

                    if(count_row==4 && i[0]==i[2] && i[1]==i[3] && t1[0]!=x && t1[1]!=x){
                        sudoku_grid_values_pencil[(x*3)+0][i[0]][k] = 0;
                        sudoku_grid_values_pencil[(x*3)+1][i[0]][k] = 0;
                        sudoku_grid_values_pencil[(x*3)+2][i[0]][k] = 0;
                        sudoku_grid_values_pencil[(x*3)+0][i[1]][k] = 0;
                        sudoku_grid_values_pencil[(x*3)+1][i[1]][k] = 0;
                        sudoku_grid_values_pencil[(x*3)+2][i[1]][k] = 0;
                    }

                    if(count_column==4 && j[0]==j[2] && j[1]==j[3] && t2[0]!=y && t2[1]!=y){
                        sudoku_grid_values_pencil[j[0]][(y*3)+0][k] = 0;
                        sudoku_grid_values_pencil[j[0]][(y*3)+1][k] = 0;
                        sudoku_grid_values_pencil[j[0]][(y*3)+2][k] = 0;
                        sudoku_grid_values_pencil[j[1]][(y*3)+0][k] = 0;
                        sudoku_grid_values_pencil[j[1]][(y*3)+1][k] = 0;
                        sudoku_grid_values_pencil[j[1]][(y*3)+2][k] = 0;
                    }
                }
            }
        }

        //paused

        for(int i = 0 ; i < 9 ; i++){
            for(int j = 0 ; j < 9 ; j++){
                printf("\n%d->", sudoku[i][j]);
                for(int k = 0 ; k < 9 ; k++){
                    printf("%2d", sudoku_grid_values_pencil[i][j][k]);
                }
            }
            printf("\n");
        }

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

        for(int i = 0 ; i < 9 ; i++){
            printf("\n");
            for(int j = 0 ; j < 9 ; j++){
                printf("%3d", sudoku[i][j]);
            }
        }
        printf("\n");
    }

    if(no_solution==1){
        printf("No solution\n");
    }

    printf("\n");
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            printf("%3d", sudoku[i][j]);
        }
        printf("\n");
    }
}
#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>

constexpr auto NC = 1.0e+10;

//using std::vector;

inline void
tension2(
        int r,
        std::vector<std::vector<float>> &res,
        std::vector<std::vector<float>> &current,
        std::vector<std::vector<float>> &volt,
        std::vector<std::vector<float>> &fall,
        std::vector<std::vector<float>> &tensi,
        std::vector<std::vector<float>> &tensf
)
{
    int i, j, ii, jj, iii, jjj, k, end;

    /*CALCULATE THE VOLTAGE FALLS ON EACH RESISTANCE*/
    /*I create the fall matrix that saves me the potential drops in every point and I initialize it to zero*/

    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            fall[i][j]=0;


    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(res[i][j]!=0 && current[i][j]!=0) /*eye if you change*/
                fall[i][j]= current[i][j]*res[i][j];

    /*I CALCULATE THE VOLTAGE IN A BATTERY-SHIRT*/
    /*I try where is the battery (the last one I find)*/
    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(volt[i][j]!=0 && current[i][j]!=0){
                /*printf("%d\t%d\n", i, j);*/
                ii=i;
                jj=j;
            }
    /*I try where is the battery (the last one I find)*/

    /*
       I initialize everything to NC (not yet calculated)*/
    for(i=0; i<r; i++)
        for(j=0; j<r; j++){
            tensf[i][j]=NC;
            tensi[i][j]=NC;
        }

    tensf[ii][jj]=volt[ii][jj]-fall[ii][jj]; /*I calculate the final voltage in the branch with the battery*/
    iii=ii; /*I calculate the final voltage in the branch with the battery*/
    jjj=jj;

    while(jj!=j){
        i=ii; /*I save the previous branch*/
        j=jj;
        for(k=0; k<r; k++)         /*I'm looking for the next branch*/
            if(current[jj][k]!=0 && tensi[jj][k]==NC){
                ii=jj;
                jj=k;
                /*printf("%d\t%d\n", ii+1, jj+1);*/
                break;
            }
        if((ii!=iii || jj!=jjj) && (ii!=i || jj!=j)){
            tensi[ii][jj]=tensf[i][j]; /*I calculate the voltages*/
            tensf[ii][jj]=tensi[ii][jj]+volt[ii][jj]-fall[ii][jj];
        }
        /*printf("tensi%f\ttensf%f\n", tensi[ii][jj], tensf[ii][jj]);*/
    }
    /*now I focus on the initial voltage of the branch with the battery*/
    for(i=0; i<r; i++)
        if(current[i][iii]!=0 && tensf[i][iii]!=NC)
            tensi[iii][jjj]=tensf[i][iii];

    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(current[i][j]==0){
                tensf[i][j]=0;
                tensi[i][j]=0;
            }


    /*CALCULATE THE VOLTAGE IN THE OTHER BRANCHES*/
    end=1;
    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(tensi[i][j]==NC)
                end=0;

    while(end==0){
        /*I'm looking for a branch where the voltage is still to be calculated (the last one I find)*/
        for(i=0; i<r; i++)
            for(j=0; j<r; j++)
                if(tensi[i][j]==NC && current[i][j]!=0){
                    ii=i; /*ii è il punto da cui parte la corrente*/
                    jj=j; /*jj il punto in cui arriva*/
                }
        /*printf("%d\t%d\n", ii+1, jj+1);*/
        /*
           I see if this branch has an adjacent branch in which the voltage has already been calculated, if so, calculate the voltage of the new branch*/
        k=-1; /*k is used to indicate whether or not I have found a nearby branch with the calculated voltage*/
        for(j=0; j<r; j++)
            if(tensi[ii][j]!=NC && tensi[ii][j]!=-NC && current[ii][j]!=0){
                tensi[ii][jj]= tensi[ii][j];
                tensf[ii][jj]= tensi[ii][jj]-fall[ii][jj]+volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(tensi[i][j]==-NC)
                            tensi[i][j]= NC;
                /*printf("a\n");*/
                break;
            }
            else if(tensi[j][ii]!=NC && tensi[j][ii]!=-NC && current[j][ii]!=0){
                tensi[ii][jj]= tensf[j][ii];
                tensf[ii][jj]= tensi[ii][jj]-fall[ii][jj]+volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(tensi[i][j]==-NC)
                            tensi[i][j]= NC;
                /*printf("b\n");*/
                break;
            }
            else if(tensi[jj][j]!=NC && tensi[jj][j]!=-NC && current[jj][j]!=0){
                tensf[ii][jj]= tensi[jj][j];
                tensi[ii][jj]= tensf[ii][jj]+fall[ii][jj]-volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(tensi[i][j]==-NC)
                            tensi[i][j]= NC;
                /*printf("c\n");*/
                break;
            }
            else if(tensi[j][jj]!=NC && tensi[j][jj]!=-NC && current[j][jj]!=0){
                tensf[ii][jj]= tensf[j][jj];
                tensi[ii][jj]= tensf[ii][jj]+fall[ii][jj]-volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(tensi[i][j]==-NC)
                            tensi[i][j]= NC;
                /*printf("d\n");*/
                break;
            }
            else if(k==-1 && j==r-1){ /*this cycle considers the case in which there is no nearby branch in which the voltage has already been calculated*/
                /*when I find a branch that has no calculated neighbors I have to report it so that, until it calculates the voltage in another branch*/
                /*you don't control it anymore, this to avoid infinite loops. To do this, mark with -NC these branches when helmet in case e, */
                /*when, on the other hand, I helmet in other cases in which I calculate a branch, I reset all the -NCs to NC */
                tensi[ii][jj]=-NC;
            }

        end=1;

        for(i=0; i<r; i++)
            for(j=0; j<r; j++)
                if(tensi[i][j]==NC && current[i][j]!=0){
                    end=0;
                    /*printf("end%d\n", end);*/
                }
        /*fine while*/
    }


    /*check physical laws*/
    /*printf("%f\n", tensi[iii][jjj]);*/
    if(tensi[iii][jjj]<=0.0001 && tensi[iii][jjj]>=-0.0001)
        printf("\nThe values obtained for the voltages respect the physical laws!\n");
}

inline std::vector<std::vector<float>> makeSquareMatrix(int r2)
{
    using std::vector;
    vector<vector<float>> res2(r2, vector<float>(r2));
    return res2;
}

inline void
saveTensionTo(
        std::ostream &fs,
        int r,
        const vector<vector<float>> &tensi,
        const vector<vector<float>> &tensf,
        const vector<vector<float>> &fall
)
{
    fs.setf(std::ios::fixed);
    int i,j;

    /*Scrivo su file*/
    fs << "The values ​​obtained for the voltages respect the physical laws\n";
    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << tensi[i][j] << "\t";
        }
        fs << tensi[i][r-1] << "\n";
    }
    fs << "The values ​​of the voltages at the end of the branch are saved in the final voltage matrix.\n";

    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << tensf[i][j] << "\t";
        }
        fs << tensf[i][r-1] << "\n";
    }
    fs << "The potential drops on each resistance are saved in the potential drop matrix..\n";
    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << fall[i][j] << "\t";
        }
        fs << fall[i][r-1] << "\n";
    }

    if (!fs)
        std::cout << "\nC'è stato un errore nella scrittura del file 'tension.dat'!\n";
    else
        printf("\nThe tension.dat file was written correctly!\n");
}

inline vector<vector<float>> makeMatrix(int r2)
{
    vector<vector<float>> res2(r2, vector<float>(r2));
    return res2;
}

//the tension function write the output to a string
inline void tension(int r, vector<vector<float>> &res, vector<vector<float>> &current, vector<vector<float>> &volt, std::ostream &fs)
{
    auto fall = makeSquareMatrix(r);
    auto tensi = makeSquareMatrix(r);
    auto tensf = makeSquareMatrix(r);
    tension2(r, res, current, volt, fall, tensi, tensf);
    saveTensionTo(fs, r, tensi, tensf, fall);
}




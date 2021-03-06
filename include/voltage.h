/*!
 * The voltage class calculate voltage for each resistance
 */

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
voltage2(
        int r,
        std::vector<std::vector<float>> &res,
        std::vector<std::vector<float>> &current,
        std::vector<std::vector<float>> &volt,
        std::vector<std::vector<float>> &fall,
        std::vector<std::vector<float>> &ivolt,
        std::vector<std::vector<float>> &fvolt
)
{
    int i, j, ii, jj, iii, jjj, k, end;

    //CALCULATE THE VOLTAGE FALLS ON EACH RESISTANCE
    //Create the fall matrix that saves me the potential drops in every point and I initialize it to zero

    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            fall[i][j]=0;


    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(res[i][j]!=0 && current[i][j]!=0) /*eye if you change*/
                fall[i][j]= current[i][j]*res[i][j];

    //CALCULATE THE VOLTAGE IN A BATTERY-SHIRT
    //Try where is the battery (the last one I find)
    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(volt[i][j]!=0 && current[i][j]!=0){
                ii=i;
                jj=j;
            }
    //Try where is the battery (the last one I find)

    //I initialize everything to NC (not yet calculated)
    for(i=0; i<r; i++)
        for(j=0; j<r; j++){
            fvolt[i][j]=NC;
            ivolt[i][j]=NC;
        }

    fvolt[ii][jj]=volt[ii][jj]-fall[ii][jj]; //calculate the final voltage in the branch with the battery
    iii=ii; //calculate the final voltage in the branch with the battery
    jjj=jj;

    while(jj!=j){
        i=ii; //I save the previous branch
        j=jj;
        for(k=0; k<r; k++)         //I'm looking for the next branch
            if(current[jj][k]!=0 && ivolt[jj][k]==NC){
                ii=jj;
                jj=k;
                break;
            }
        if((ii!=iii || jj!=jjj) && (ii!=i || jj!=j)){
            ivolt[ii][jj]=fvolt[i][j]; //I calculate the voltages
            fvolt[ii][jj]=ivolt[ii][jj]+volt[ii][jj]-fall[ii][jj];
        }
    }
    //now I focus on the initial voltage of the branch with the battery
    for(i=0; i<r; i++)
        if(current[i][iii]!=0 && fvolt[i][iii]!=NC)
            ivolt[iii][jjj]=fvolt[i][iii];

    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(current[i][j]==0){
                fvolt[i][j]=0;
                ivolt[i][j]=0;
            }


    //CALCULATE THE VOLTAGE IN THE OTHER BRANCHES
    end=1;
    for(i=0; i<r; i++)
        for(j=0; j<r; j++)
            if(ivolt[i][j]==NC)
                end=0;

    while(end==0){
        //I'm looking for a branch where the voltage is still to be calculated (the last one I find)
        for(i=0; i<r; i++)
            for(j=0; j<r; j++)
                if(ivolt[i][j]==NC && current[i][j]!=0){
                    ii=i; /*ii is the point from which the current starts*/
                    jj=j; /*jj the point where it arrives*/
                }

           //I see if this branch has an adjacent branch in which the voltage has already been calculated,
           // if so, calculate the voltage of the new branch
        k=-1; //k is used to indicate whether or not I have found a nearby branch with the calculated voltage
        for(j=0; j<r; j++)
            if(ivolt[ii][j]!=NC && ivolt[ii][j]!=-NC && current[ii][j]!=0){
                ivolt[ii][jj]= ivolt[ii][j];
                fvolt[ii][jj]= ivolt[ii][jj]-fall[ii][jj]+volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(ivolt[i][j]==-NC)
                            ivolt[i][j]= NC;
                /*printf("a\n");*/
                break;
            }
            else if(ivolt[j][ii]!=NC && ivolt[j][ii]!=-NC && current[j][ii]!=0){
                ivolt[ii][jj]= fvolt[j][ii];
                fvolt[ii][jj]= ivolt[ii][jj]-fall[ii][jj]+volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(ivolt[i][j]==-NC)
                            ivolt[i][j]= NC;
                break;
            }
            else if(ivolt[jj][j]!=NC && ivolt[jj][j]!=-NC && current[jj][j]!=0){
                fvolt[ii][jj]= ivolt[jj][j];
                ivolt[ii][jj]= fvolt[ii][jj]+fall[ii][jj]-volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(ivolt[i][j]==-NC)
                            ivolt[i][j]= NC;
                break;
            }
            else if(ivolt[j][jj]!=NC && ivolt[j][jj]!=-NC && current[j][jj]!=0){
                fvolt[ii][jj]= fvolt[j][jj];
                ivolt[ii][jj]= fvolt[ii][jj]+fall[ii][jj]-volt[ii][jj];
                k=1;
                for(i=0; i<r; i++)
                    for(j=0; j<r; j++)
                        if(ivolt[i][j]==-NC)
                            ivolt[i][j]= NC;
                /*printf("d\n");*/
                break;
            }
            else if(k==-1 && j==r-1){ /*this cycle considers the case in which there is no nearby branch in which the voltage has already been calculated*/
                /*when I find a branch that has no calculated neighbors I have to report it so that, until it calculates the voltage in another branch*/
                /*you don't control it anymore, this to avoid infinite loops. To do this, mark with -NC these branches when helmet in case e, */
                /*when, on the other hand, I helmet in other cases in which I calculate a branch, I reset all the -NCs to NC */
                ivolt[ii][jj]=-NC;
            }

        end=1;

        for(i=0; i<r; i++)
            for(j=0; j<r; j++)
                if(ivolt[i][j]==NC && current[i][j]!=0){
                    end=0;
                }
    }


    //check physical laws
    if(ivolt[iii][jjj]<=0.0001 && ivolt[iii][jjj]>=-0.0001)
        printf("\nThe values obtained for the voltages respect the physical laws!\n");
}

inline std::vector<std::vector<float>> makeSquareMatrix(int r2)
{
    using std::vector;
    vector<vector<float>> res2(r2, vector<float>(r2));
    return res2;
}

inline void
saveVoltageTo(
        std::ostream &fs,
        int r,
        const vector<vector<float>> &ivolt,
        const vector<vector<float>> &fvolt,
        const vector<vector<float>> &fall
)
{
    fs.setf(std::ios::fixed);
    int i,j;

    /*Scrivo su file*/
    fs << "The values ​​obtained for the voltages respect the physical laws\n";
    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << ivolt[i][j] << "\t";
        }
        fs << ivolt[i][r-1] << "\n";
    }
    fs << "The values ​​of the voltages at the end of the branch are saved in the final voltage matrix.\n";

    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << fvolt[i][j] << "\t";
        }
        fs << fvolt[i][r-1] << "\n";
    }
    fs << "The potential drops on each resistance are saved in the potential drop matrix..\n";
    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << fall[i][j] << "\t";
        }
        fs << fall[i][r-1] << "\n";
    }

    if (!fs)
        std::cout << "\nThere was an error in writing the file 'voltage.dat'!\n";
    else
        printf("\nThe voltage.dat file was written correctly!\n");
}

inline vector<vector<float>> makeMatrix(int r2)
{
    vector<vector<float>> res2(r2, vector<float>(r2));
    return res2;
}

//the voltage function write the output to a string
inline void voltage(int r, vector<vector<float>> &res, vector<vector<float>> &current, vector<vector<float>> &volt, std::ostream &fs)
{
    auto fall = makeSquareMatrix(r);
    auto ivolt = makeSquareMatrix(r);
    auto fvolt = makeSquareMatrix(r);
    voltage2(r, res, current, volt, fall, ivolt, fvolt);
    saveVoltageTo(fs, r, ivolt, fvolt, fall);
}




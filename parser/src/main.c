/*
Lucian Tranc
1045249
ltranc@uoguelph.ca
*/

#include "SVGParser.h"

int main(int argc, char **argv)
{

    if (argc != 2)
        return(1);

    

    SVGimage* mySVG = createValidSVGimage(argv[1], "svg.xsd");

    if (mySVG == NULL) {

        printf("NULL\n");
        deleteSVGimage(mySVG);
        return 0;

    }

    /*Attribute * addAttr1 = malloc(sizeof(Attribute));

    addAttr1->name = malloc(sizeof(char) * 4);
    addAttr1->value = malloc(sizeof(char) * 4);

    strcpy(addAttr1->name, "big");
    strcpy(addAttr1->value, "gay");

   
    setAttribute(mySVG, SVG_IMAGE, 0, addAttr1);
    Attribute * addAttr2 = malloc(sizeof(Attribute));

    addAttr2->name = malloc(10);
    addAttr2->value = malloc(10);

    strcpy(addAttr2->name, "title");
    strcpy(addAttr2->value, "newtite");

    setAttribute(mySVG, SVG_IMAGE, 0, addAttr2);
    Attribute * addAttr3 = malloc(sizeof(Attribute));

    addAttr3->name = malloc(10);
    addAttr3->value = malloc(10);

    strcpy(addAttr3->name, "x");
    strcpy(addAttr3->value, "69");

    setAttribute(mySVG, RECT, 1, addAttr3);
 
    Attribute * addAttr4 = malloc(sizeof(Attribute));

    addAttr4->name = malloc(10);
    addAttr4->value = malloc(10);

    strcpy(addAttr4->name, "newx");
    strcpy(addAttr4->value, "69");

    setAttribute(mySVG, CIRC, 4, addAttr4);

    
    Attribute * addAttr5 = malloc(sizeof(Attribute));

    addAttr5->name = malloc(10);
    addAttr5->value = malloc(10);

    strcpy(addAttr5->name, "dl");
    strcpy(addAttr5->value, "NEW DATA");

    setAttribute(mySVG, PATH, 0, addAttr5); 

    Attribute * addAttr6 = malloc(sizeof(Attribute));

    addAttr6->name = malloc(10);
    addAttr6->value = malloc(10);

    strcpy(addAttr6->name, "fill");
    strcpy(addAttr6->value, "NEW DATA");

    setAttribute(mySVG, GROUP, 0, addAttr6);


    Rectangle* newRect = malloc(sizeof(Rectangle));
    strcpy(newRect->units, "");
    newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    newRect->x = 5;
    newRect->y = 6;
    newRect->width = 7;
    newRect->height = 8;

    addComponent(mySVG, RECT, newRect);

    if (validateSVGimage(mySVG, "svg.xsd"))
        printf("valid\n");
    else
        printf("not valid\n");

    writeSVGimage(mySVG, "testfilewattr.svg");
    
    char * attrlist = attrListToJSON(mySVG->otherAttributes);
    printf("attrlist: %s\n", attrlist);
    free(attrlist);

    char * circlist = circListToJSON(mySVG->circles);
    printf("circlist: %s\n", circlist);
    free(circlist);

    char * rectlist = rectListToJSON(mySVG->rectangles);
    printf("rectlist: %s\n", rectlist);
    free(rectlist);

    char * pathlist = pathListToJSON(mySVG->paths);
    printf("pathlist: %s\n", pathlist);
    free(pathlist);

    char * grouplist = groupListToJSON(mySVG->groups);
    printf("grouplist: %s\n", grouplist);
    free(grouplist);

    char * svg = SVGtoJSON(mySVG);
    printf("svg: %s\n", svg);
    free(svg);
 
    char* description = SVGimageToString(mySVG);
    printf("\n\n%s\n", description);
    free(description);

    List* rectList = getRects(mySVG);
    char * rectString = toString(rectList);
    printf("\n\nRect String: \n%s\n", rectString);
    free(rectString);
    freeList(rectList);

    List* circList = getCircles(mySVG);
    char * circString = toString(circList);
    printf("\n\nCirc String: \n%s\n", circString);
    free(circString);
    freeList(circList);

    List* pathList = getPaths(mySVG);
    char * pathString = toString(pathList);
    printf("\n\nPath String: \n%s\n", pathString);
    free(pathString);
    freeList(pathList);
    
    List* groupList = getGroups(mySVG);
    char * groupString = toString(groupList);
    printf("\n\nGroup String: \n%s\n", groupString);
    free(groupString);
    freeList(groupList);

    char * pathData;

    pathData = malloc(strlen("M200,300 Q400,50 600,300 T1000,300") + 1);

    strcpy(pathData, "M200,300 Q400,50 600,300 T1000,300");

    printf("\nnumber of rectangles with an area of 716404: %d\n", numRectsWithArea(mySVG, 716404));

    printf("\nnumber of circles with an area of x: %d\n", numCirclesWithArea(mySVG, 40.72));

    printf("\nnumber of paths with an area of x: %d\n", numPathsWithdata(mySVG, pathData));

    printf("\nnumber of groups of length 3: %d\n", numGroupsWithLen(mySVG, 3));

    printf("\nnumber of attributes in SVG: %d\n", numAttr(mySVG));

    free(pathData);
*/
    deleteSVGimage(mySVG);
/*

    printf("\n\n\tFIRST TEST DONE \n\n");


    SVGimage * image;
    image = createValidSVGimage("Test_Harness_A2_2750/bin/vest.svg", "Test_Harness_A2_2750/bin/svg.xsd");
    deleteSVGimage(image);
 
    printf("\n\n\tSECOND TEST DONE \n\n");

    JSONtoSVG("{\"title\":\"stuff\",\"descr\":\"junk\"}");

    printf("\n\n\n"); */


    return 0;
}
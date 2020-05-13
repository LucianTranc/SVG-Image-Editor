/*
Lucian Tranc
1045249
ltranc@uoguelph.ca
*/

#include "SVGParser.h"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <strings.h>
/*export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./bin*/

/*A1*/
char* stringdup(const char * str);
char * getUnits(char * str);
char * getValue(char * str);
static void buildSVG(xmlNode * a_node, SVGimage * mySVG);
Rectangle* initRect(xmlNode * cur_node);
Circle* initCirc(xmlNode * cur_node);
Path* initPath(xmlNode * cur_node);
void initGroup(xmlNode * cur_node, Group * myGroup);
SVGimage* initSVG(xmlDoc *doc);
void getGroupRects(void * theGroup, List * rectList);
void getGroupCircles(void * theGroup, List * circList);
void getGroupGroups(void * theGroup, List * groupList);
void getGroupPaths(void * theGroup, List * pathList);
void findLenOfGroup(Group * group, int * count, int len);
char* floatToString(float num, char * units);
char* intToString(int num, char * units);
/*A2*/
xmlDocPtr buildXMLtree(SVGimage * img);
void addRectsToTree(SVGimage * img, xmlNodePtr root_node);
void addCircsToTree(SVGimage * img, xmlNodePtr root_node);
void addPathsToTree(SVGimage * img, xmlNodePtr root_node);
void addGroupsToTree(SVGimage * img, xmlNodePtr root_node);
void addGroupRectsToTree(Group * group, xmlNodePtr root_node);
void addGroupCircsToTree(Group * group, xmlNodePtr root_node);
void addGroupPathsToTree(Group * group, xmlNodePtr root_node);
void addGroupGroupsToTree(Group * group, xmlNodePtr root_node);
bool validateSchema(SVGimage * img, char * schemaFile);
bool validateRects(List * rectList);
bool validateCircs(List * circList);
bool validatePaths(List * pathList);
bool validateGroups(List * groupList);
void setSVGAttribute(SVGimage * image, Attribute* newAttribute);
void setRectAttribute(Rectangle * rect, Attribute* newAttribute);
void setCircleAttribute(Circle * circ, Attribute* newAttribute);
void setPathAttribute(Path * path, Attribute* newAttribute);
void setGroupAttribute(Group * group, Attribute* newAttribute);
/*A3*/
char * SVGtoJSONwrapper(char * fileName);




char* stringdup(const char * str) {

    char * returnString;

    returnString = malloc(strlen(str) + 1);

    strcpy(returnString, str);

    return returnString;

}

char * getUnits(char * str) {

    int i;

    for (i = 0; i < strlen(str); i++) {

        if (isdigit(str[i]) || str[i] == '.' || str[i] == '-')
            continue;
        else 
            break;

    }

    str += i;

    return str;

}

char * getValue(char * str) {

    int i;

    for (i = 0; i < strlen(str); i++) {

        if (isdigit(str[i]) || str[i] == '.' || str[i] == '-')
            continue;
        else 
            break;

    }

    str[i] = '\0';

    return str;

}

static void buildSVG(xmlNode * a_node, SVGimage * mySVG)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            //printf("node type: Element, name: %s\n", cur_node->name);
            
            if (strcmp((const char *)cur_node->name, "title") == 0 && cur_node->children->content != NULL) {
                
                if (strlen((char *)cur_node->children->content) < 256)
                    strcpy(mySVG->title, (char *)cur_node->children->content);
                else if (strlen((char *)cur_node->children->content) >= 256) {
                    strncpy(mySVG->title, (char *)cur_node->children->content, 255);
                    mySVG->title[255] = '\0';
                }
                
            }
            if (strcmp((const char *)cur_node->name, "desc") == 0 && cur_node->children->content != NULL) {
                
                if (strlen((char *)cur_node->children->content) < 256)
                    strcpy(mySVG->description, (char *)cur_node->children->content);
                else if (strlen((char *)cur_node->children->content) >= 256) {
                    strncpy(mySVG->description, (char *)cur_node->children->content, 255);
                    mySVG->description[255] = '\0';
                }

            }
            if (strcmp((const char *)cur_node->name, "rect") == 0) {

                Rectangle* newRect = initRect(cur_node);
                insertBack(mySVG->rectangles, (void*)newRect);

            }
            if (strcmp((const char *)cur_node->name, "path") == 0) {
                
                Path* newPath = initPath(cur_node);
                insertBack(mySVG->paths, (void*)newPath);

            }
            if (strcmp((const char *)cur_node->name, "circle") == 0) {

                Circle* newCirc = initCirc(cur_node);
                insertBack(mySVG->circles, (void*)newCirc);

            }
            if (strcmp((const char *)cur_node->name, "g") == 0) {

                Group* newGroup = malloc(sizeof(Group));

                newGroup->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
                newGroup->circles = initializeList(circleToString, deleteCircle, compareCircles);
                newGroup->paths = initializeList(pathToString, deletePath, comparePaths);
                newGroup->groups = initializeList(groupToString, deleteGroup, compareGroups);
                newGroup->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

                xmlAttr *attr;

                for (attr = cur_node->properties; attr != NULL; attr = attr->next)
                {
                    xmlNode *value = attr->children;

                    Attribute* newAttr = malloc(sizeof(Attribute));
                    newAttr->name = stringdup((char*)attr->name);
                    newAttr->value = stringdup((char*)value->content);
                    insertBack(newGroup->otherAttributes, (void *)newAttr);

                }

                initGroup(cur_node->children, newGroup);

                insertBack(mySVG->groups, (void*)newGroup);

            }
        }        

        //buildSVG(cur_node->children, mySVG);
    }

    //printf("SVG BUILT\n");

}

void initGroup(xmlNode * a_node, Group * myGroup)
{

    //printf("\nNEW GROUP\n");

    xmlNode *cur_node = NULL;


    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            //printf("node type in group: Element, name: %s\n", cur_node->name);
            
            if (strcmp((const char *)cur_node->name, "rect") == 0) {

                Rectangle* newRect = initRect(cur_node);
                insertBack(myGroup->rectangles, (void*)newRect);

            }
            if (strcmp((const char *)cur_node->name, "path") == 0) {
                
                Path* newPath = initPath(cur_node);
                insertBack(myGroup->paths, (void*)newPath);

            }
            if (strcmp((const char *)cur_node->name, "circle") == 0) {

                Circle* newCirc = initCirc(cur_node);
                insertBack(myGroup->circles, (void*)newCirc);

            }
            if (strcmp((const char *)cur_node->name, "g") == 0) {

                Group* newGroup = malloc(sizeof(Group));

                newGroup->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
                newGroup->circles = initializeList(circleToString, deleteCircle, compareCircles);
                newGroup->paths = initializeList(pathToString, deletePath, comparePaths);
                newGroup->groups = initializeList(groupToString, deleteGroup, compareGroups);
                newGroup->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

                xmlAttr *attr;

                for (attr = cur_node->properties; attr != NULL; attr = attr->next)
                {
                    xmlNode *value = attr->children;

                    Attribute* newAttr = malloc(sizeof(Attribute));
                    newAttr->name = stringdup((char*)attr->name);
                    newAttr->value = stringdup((char*)value->content);
                    insertBack(newGroup->otherAttributes, (void *)newAttr);

                }

                initGroup(cur_node->children, newGroup);

                insertBack(myGroup->groups, (void*)newGroup);

            }
        }        

    }
}


Rectangle* initRect(xmlNode * cur_node) {

    Rectangle* newRect = malloc(sizeof(Rectangle));
    strcpy(newRect->units, "");
    newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    newRect->x = 0;
    newRect->y = 0;
    newRect->width = 0;
    newRect->height = 0;

    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {

        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);
        //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);

        if (strcmp(attrName, "x") == 0) {

            char * units = getUnits(cont);
            //printf("units: %s\n", units);
            if (strlen(units) < 50 && strcmp(newRect->units, "") == 0)
                strcpy(newRect->units, units);
            else if (strlen(units) >= 50 && strcmp(newRect->units, "") == 0) {
                strncpy(newRect->units, units, 49);
                newRect->units[49] = '\0';
            }
            char * xvalue = getValue(cont);
            //printf("x value: %s\n", xvalue);
            newRect->x = strtof(xvalue, NULL);  

        }
        else if (strcmp(attrName, "y") == 0) {

            char * units = getUnits(cont);
            //printf("units: %s\n", units);
            if (strlen(units) < 50 && strcmp(newRect->units, "") == 0)
                strcpy(newRect->units, units);
            else if (strlen(units) >= 50 && strcmp(newRect->units, "") == 0) {
                strncpy(newRect->units, units, 49);
                newRect->units[49] = '\0';
            }
            char * yvalue = getValue(cont);
            //printf("y value: %s\n", yvalue);
            newRect->y = strtof(yvalue, NULL);  

        }
        else if (strcmp(attrName, "width") == 0) {

            char * units = getUnits(cont);
            //printf("units: %s\n", units);
            if (strlen(units) < 50 && strcmp(newRect->units, "") == 0)
                strcpy(newRect->units, units);
            else if (strlen(units) >= 50 && strcmp(newRect->units, "") == 0) {
                strncpy(newRect->units, units, 49);
                newRect->units[49] = '\0';
            }
            char * wvalue = getValue(cont);
            //printf("width value: %s\n", wvalue);
            newRect->width = strtof(wvalue, NULL);  

        }
        else if (strcmp(attrName, "height") == 0) {

            char * units = getUnits(cont);
            //printf("units: %s\n", units);
            if (strlen(units) < 50 && strcmp(newRect->units, "") == 0)
                strcpy(newRect->units, units);
            else if (strlen(units) >= 50 && strcmp(newRect->units, "") == 0) {
                strncpy(newRect->units, units, 49);
                newRect->units[49] = '\0';
            }
            char * hvalue = getValue(cont);
            //printf("height value: %s\n", hvalue);
            newRect->height = strtof(hvalue, NULL);  

        }
        else {

            Attribute* newAttr = malloc(sizeof(Attribute));
            newAttr->name = stringdup((char*)attrName);
            newAttr->value = stringdup((char*)cont);
            insertBack(newRect->otherAttributes, (void*)newAttr);

        }

    }

    return newRect;

}

Circle * initCirc(xmlNode * cur_node) {

    Circle* newCirc = malloc(sizeof(Circle));
    strcpy(newCirc->units, "");
    newCirc->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    newCirc->cx = 0;
    newCirc->cy = 0;
    newCirc->r = 0;

    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {

        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);
        //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);

        if (strcmp(attrName, "cx") == 0) {

            char * units = getUnits(cont);
            //printf("units: %s\n", units);
            if (strlen(units) < 50)
                strcpy(newCirc->units, units);
            else if (strlen(units) >= 50) {
                strncpy(newCirc->units, units, 49);
                newCirc->units[49] = '\0';
            }
            char * xvalue = getValue(cont);
            newCirc->cx = strtof(xvalue, NULL);

        }
        else if (strcmp(attrName, "cy") == 0) {

            char * units = getUnits(cont);
            //printf("units: %s\n", units);
            if (strlen(units) < 50)
                strcpy(newCirc->units, units);
            else if (strlen(units) >= 50) {
                strncpy(newCirc->units, units, 49);
                newCirc->units[49] = '\0';
            }
            char * yvalue = getValue(cont);
            newCirc->cy = strtof(yvalue, NULL);  


        }
        else if (strcmp(attrName, "r") == 0) {

            char * units = getUnits(cont);
            //printf("units: %s\n", units);
            if (strlen(units) < 50)
                strcpy(newCirc->units, units);
            else if (strlen(units) >= 50) {
                strncpy(newCirc->units, units, 49);
                newCirc->units[49] = '\0';
            }
            char * rvalue = getValue(cont);
            //printf("r value: %s\n", rvalue);
            newCirc->r = strtof(rvalue, NULL);  
            

        }
        else {

            Attribute* newAttr = malloc(sizeof(Attribute));
            newAttr->name = stringdup((char*)attrName);
            newAttr->value = stringdup((char*)cont);
            insertBack(newCirc->otherAttributes, (void*)newAttr);

        }

    }
    
    return newCirc;

}

Path * initPath(xmlNode * cur_node) {

    Path* newPath = malloc(sizeof(Path));
    newPath->data = (char*)malloc(sizeof(char));
    strcpy(newPath->data, "");

    newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);


    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {

        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);
        //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);

        
        if (strcmp(attrName, "d") == 0) {

            newPath->data = realloc(newPath->data, strlen(cont) + 1);
            
            strcpy(newPath->data, cont); 

        }
        else {

            Attribute* newAttr = malloc(sizeof(Attribute));
            newAttr->name = stringdup((char*)attrName);
            newAttr->value = stringdup((char*)cont);
            insertBack(newPath->otherAttributes, (void*)newAttr);

        }

    }
    
    return newPath;

}

SVGimage* initSVG(xmlDoc *doc) {

    xmlNode *root_element = NULL;

    root_element = xmlDocGetRootElement(doc);

    SVGimage* mySVG = malloc(sizeof(SVGimage));

    mySVG->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    mySVG->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    mySVG->circles = initializeList(circleToString, deleteCircle, compareCircles);
    mySVG->paths = initializeList(pathToString, deletePath, compareCircles);
    mySVG->groups = initializeList(groupToString, deleteGroup, compareGroups);


    strcpy(mySVG->title, ""); 
    strcpy(mySVG->description, ""); 
    strcpy(mySVG->namespace, "");

    if ((const char*)root_element->nsDef == NULL) {

        freeList(mySVG->otherAttributes);
        freeList(mySVG->rectangles);
        freeList(mySVG->circles);
        freeList(mySVG->paths);
        freeList(mySVG->groups);

        free(mySVG);

        return NULL;

    }

    if ((strcmp((const char*)root_element->nsDef->href, "") != 0) && (strlen((const char*)root_element->nsDef->href) < 256)) {

        strcpy(mySVG->namespace, (const char*)root_element->nsDef->href); 

    }
    else if ((strcmp((const char*)root_element->nsDef->href, "") != 0) && (strlen((const char*)root_element->nsDef->href) >= 256)) {

        strncpy(mySVG->namespace, (const char*)root_element->nsDef->href, 255);
        mySVG->namespace[255] = '\0';

    }
    else {

        freeList(mySVG->otherAttributes);
        freeList(mySVG->rectangles);
        freeList(mySVG->circles);
        freeList(mySVG->paths);
        freeList(mySVG->groups);

        free(mySVG);

        return NULL;

    }


    xmlAttr *attr;

    for (attr = root_element->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;

        Attribute* newAttr = malloc(sizeof(Attribute));
        newAttr->name = stringdup((char*)attr->name);
        newAttr->value = stringdup((char*)value->content);
        insertBack(mySVG->otherAttributes, (void *)newAttr);

    }

    return mySVG;

}



SVGimage* createSVGimage(char* fileName) {

    if (fileName == NULL) {

        return NULL;

    }

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    LIBXML_TEST_VERSION
    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        //printf("error: could not parse file %s\n", fileName);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    /*Initialize the SVGimage structure */

    SVGimage* mySVG = initSVG(doc);

    if (mySVG == NULL) {

        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;

    }

    /*Get the root element node */

    root_element = xmlDocGetRootElement(doc);

    buildSVG(root_element->children, mySVG);

    /*free the document */

    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return mySVG;


}

char* SVGimageToString(SVGimage* img) {

    if (img == NULL) {

        char * emptyString = malloc(2);
        strcpy(emptyString, "");
        return emptyString;

    }

    int stringLength = 0;

    char* returnString;
    char* attrString = toString(img->otherAttributes);
    char* rectString = toString(img->rectangles);
    char* circString = toString(img->circles);
    char* pathString = toString(img->paths);
    char* groupString = toString(img->groups);

    stringLength += strlen("SVG Title: \nSVG Description: \nSVG Attributes: \nNameSpace: \nRectangles: \nCircles: \nPaths: \nGroups: \n");
    stringLength += strlen(img->title);
    stringLength += strlen(img->description);
    stringLength += strlen(attrString);
    stringLength += strlen(rectString);
    stringLength += strlen(circString);
    stringLength += strlen(pathString);
    stringLength += strlen(groupString);
    stringLength += strlen(img->namespace);

    returnString = malloc(stringLength + 1);

    sprintf(returnString,   "SVG Title: %s\nSVG Description: %s\nSVG Attributes: %s\nNameSpace: %s\nRectangles: %s\nCircles: %s\nPaths: %s\nGroups: %s\n",
                            img->title, img->description, attrString, img->namespace, rectString, circString, pathString, groupString);

    free(attrString);
    free(rectString);
    free(circString);
    free(pathString);
    free(groupString);

    returnString = realloc(returnString, strlen(returnString) + 1);

    return returnString;

}

void deleteSVGimage(SVGimage* img) {

    if (img == NULL)
        return;

    if (img->otherAttributes != NULL) {

        freeList(img->otherAttributes);
        //printf("freed svg attributes\n");
    }
    if (img->rectangles != NULL){
        freeList(img->rectangles);
        //printf("freed svg rect\n");
    }

    if (img->circles != NULL){
        freeList(img->circles);
        //printf("freed svg circ\n");
    }

    if (img->paths != NULL){
        freeList(img->paths);
        //printf("freed svg path\n");
    }

    if (img->groups != NULL) {
        freeList(img->groups);
        //printf("freed svg group\n");
    }

    free(img);

}

void deleteAttribute( void* data) {

    Attribute* delete;

    if (data == NULL)
        return;

    delete = (Attribute*)data;
    
    if (delete->name != NULL)
        free(delete->name);

    if (delete->value != NULL)
        free(delete->value);

    free(data);

}

char* attributeToString( void* data) {

    if (data == NULL) {

        char * emptyString = malloc(2);
        strcpy(emptyString, "");
        return emptyString;

    }
    
    Attribute* attr;

    int stringLength = 0;

    if (data == NULL)
        return NULL;

    attr = (Attribute*)data;

    if (attr->name == NULL)
        return NULL;

    if (attr->value == NULL)
        return NULL;


    stringLength = strlen(attr->name) + strlen(": ") + strlen(attr->value);

    char * returnString;

    returnString = malloc(stringLength+1);

    sprintf(returnString, "%s: %s", attr->name, attr->value);


    return returnString;


}
int compareAttributes(const void *first, const void *second) {

    return 0;
}

void deleteGroup(void* data) {

    Group* delete;

    if (data == NULL)
        return;

    delete = (Group*)data;

    if (delete->otherAttributes != NULL) 
        freeList(delete->otherAttributes);

    if (delete->rectangles != NULL) 
        freeList(delete->rectangles);

    if (delete->circles != NULL) 
        freeList(delete->circles);

    if (delete->paths != NULL) 
        freeList(delete->paths);

    if (delete->groups != NULL) 
        freeList(delete->groups);
    
    free(delete);

}
char* groupToString( void* data) {

    int stringLength = 0;

    Group* group = (Group*)data;

    char* returnString;
    char* attrString = toString(group->otherAttributes);
    char* rectString = toString(group->rectangles);
    char* circString = toString(group->circles);
    char* pathString = toString(group->paths);
    char* groupString = toString(group->groups);


    stringLength += strlen("New Group:\nSVG Attributes: \nRectangles: \nCircles: \nPaths: \nInner Groups: \nEnd of Group\n");
    stringLength += strlen(attrString);
    stringLength += strlen(rectString);
    stringLength += strlen(circString);
    stringLength += strlen(pathString);
    stringLength += strlen(groupString);

    returnString = malloc(stringLength + 1);


    sprintf(returnString,   "New Group:\nSVG Attributes: %s\nRectangles: %s\nCircles: %s\nPaths: %s\nInner Groups: %s\nEnd of Group\n",
                            attrString, rectString, circString, pathString, groupString);


    free(attrString);
    free(rectString);
    free(circString);
    free(pathString);
    free(groupString);

    returnString = realloc(returnString, strlen(returnString) + 1);

    return returnString;


}
int compareGroups(const void *first, const void *second) {

    return 0;

}



void deleteRectangle(void* data) {

    Rectangle* delete;

    if (data == NULL)
        return;

    delete = (Rectangle*)data;

    if (delete->otherAttributes != NULL) 
        freeList(delete->otherAttributes);
    

    free(delete);

}

char* intToString(int num, char * units) {


    char * returnString = malloc(50);

    strcpy(returnString, "");

    if (num>=0)
        sprintf(returnString, "%d", num);
    
    if (units == NULL || strlen(units) < 1)
        return returnString;
    else {
        //strcat(returnString, " ");
        strcat(returnString, units);
    }

    return returnString;

}

char* floatToString(float num, char * units) {


    char * returnString = malloc(50);

    strcpy(returnString, "");

    if (num>=0)
        sprintf(returnString, "%.2f", num);
    
    if (units == NULL || strlen(units) < 1)
        return returnString;
    else {
        //strcat(returnString, " ");
        strcat(returnString, units);
    }

    return returnString;

}

char* rectangleToString(void* data) {

    if (data == NULL)
        return NULL;

    Rectangle* rect;

    rect = (Rectangle*)data;

    int stringLength = 0;

    char* returnString;
    char* attrString = toString(rect->otherAttributes);
    char* xString = floatToString(rect->x, NULL);
    char* yString = floatToString(rect->y, NULL);
    char* heightString = floatToString(rect->height, NULL);
    char* widthString = floatToString(rect->width, NULL);

    stringLength += strlen(attrString);
    stringLength += strlen(xString);
    stringLength += strlen(yString);
    stringLength += strlen(heightString);
    stringLength += strlen(widthString);
    stringLength += strlen(rect->units);
    stringLength += strlen("x: , y: , height: , width: , units: , other: \n");

    returnString = malloc(stringLength + 1);

    sprintf(returnString, "x: %s, y: %s, height: %s, width: %s, units: %s, other: %s\n", xString, yString, heightString, widthString, rect->units, attrString);

    free(xString);
    free(yString);
    free(heightString);
    free(widthString);
    free(attrString);

    return returnString;

}
int compareRectangles(const void *first, const void *second) {

    return 0;

}



void deleteCircle(void* data) {

    Circle* delete;

    if (data == NULL)
        return;

    delete = (Circle*)data;

    if (delete->otherAttributes != NULL) 
        freeList(delete->otherAttributes);
    

    free(delete);

}
char* circleToString(void* data) {

    if (data == NULL)
        return NULL;

    Circle* circle;

    circle = (Circle*)data;

    int stringLength = 0;

    char* returnString;
    char* attrString = toString(circle->otherAttributes);
    char* cxString = floatToString(circle->cx, NULL);
    char* cyString = floatToString(circle->cy, NULL);
    char* rString = floatToString(circle->r, NULL);

    stringLength += strlen(attrString);
    stringLength += strlen(cxString);
    stringLength += strlen(cyString);
    stringLength += strlen(rString);
    stringLength += strlen(circle->units);
    stringLength += strlen("cx: , cy: , r: , units: , other: ");

    returnString = malloc(stringLength + 1);

    sprintf(returnString, "cx: %s, cy: %s, r: %s, units: %s, other: %s", cxString, cyString, rString, circle->units, attrString);

    free(cxString);
    free(cyString);
    free(rString);
    free(attrString);

    return returnString;

}
int compareCircles(const void *first, const void *second){

    return 0;

}

void deletePath(void* data) {

    Path* delete;

    if (data == NULL)
        return;

    delete = (Path*)data;

    if (delete->otherAttributes != NULL) 
        freeList(delete->otherAttributes);
    
    if (delete->data != NULL)
        free(delete->data);


    free(delete);

}
char* pathToString(void* data) {

    Path* path;

    if (data == NULL)
        return NULL;

    int stringLength = 0;

    path = (Path*)data;

    char* returnString;
    char* attrString = toString(path->otherAttributes);
    
    stringLength += strlen(attrString);
    stringLength += strlen(path->data);
    stringLength += strlen("data: , other: \n");

    returnString = malloc(stringLength + 1);

    sprintf(returnString, "data: %s, other: %s\n", path->data, attrString);

    free(attrString);

    return returnString;

}
int comparePaths(const void *first, const void *second) {

    return 0;

}

void noDelete (void* data) {

    return;

}

void getGroupRects(void * theGroup, List * rectList) {

    Group* group = (Group*)theGroup;

    ListIterator rectIter = createIterator(group->rectangles);

    void * elem;

    while((elem = nextElement(&rectIter)) != NULL) {

        insertBack(rectList, elem);

    }

    ListIterator groupIter = createIterator(group->groups);

    void * newGroup;

    while((newGroup = nextElement(&groupIter)) != NULL) {

        getGroupRects((Group*)newGroup, rectList);

    }

}

List* getRects(SVGimage* img) {

    if (img == NULL) {

        return NULL;

    }

    List* rectList = initializeList(rectangleToString, noDelete, compareRectangles);

    ListIterator iter = createIterator(img->rectangles);

    void * elem;

    while((elem = nextElement(&iter)) != NULL) {

        insertBack(rectList, elem);

    }

    iter = createIterator(img->groups);

    void * group;

    while((group = nextElement(&iter)) != NULL) {

        getGroupRects(group, rectList);

    }

    return rectList;

}

void getGroupCircles(void * theGroup, List * circList) {

    Group* group = (Group*)theGroup;

    ListIterator circIter = createIterator(group->circles);

    void * elem;

    while((elem = nextElement(&circIter)) != NULL) {

        insertBack(circList, elem);

    }

    ListIterator groupIter = createIterator(group->groups);

    void * newGroup;

    while((newGroup = nextElement(&groupIter)) != NULL) {

        getGroupCircles((Group*)newGroup, circList);

    }

}

List* getCircles(SVGimage* img) {

    if (img == NULL) {

        return NULL;

    }

    List* circList = initializeList(circleToString, noDelete, compareCircles);

    ListIterator iter = createIterator(img->circles);

    void * elem;

    while((elem = nextElement(&iter)) != NULL) {

        insertBack(circList, elem);

    }

    iter = createIterator(img->groups);

    void * group;

    while((group = nextElement(&iter)) != NULL) {

        getGroupCircles(group, circList);

    }

    return circList;

}

void getGroupGroups(void * theGroup, List * groupList) {

    Group* group = (Group*)theGroup;

    ListIterator innerGroupIter = createIterator(group->groups);

    void * elem;

    while((elem = nextElement(&innerGroupIter)) != NULL) {

        insertBack(groupList, elem);

    }

    ListIterator groupIter = createIterator(group->groups);

    void * newGroup;

    while((newGroup = nextElement(&groupIter)) != NULL) {

        getGroupGroups((Group*)newGroup, groupList);

    }

}

List* getGroups(SVGimage* img) {

    if (img == NULL) {

        return NULL;

    }

    List* groupList = initializeList(groupToString, noDelete, compareGroups);

    ListIterator iter = createIterator(img->groups);

    void * elem;

    while((elem = nextElement(&iter)) != NULL) {

        insertBack(groupList, elem);

    }

    iter = createIterator(img->groups);

    void * group;

    while((group = nextElement(&iter)) != NULL) {

        getGroupGroups(group, groupList);

    }

    return groupList;

}

void getGroupPaths(void * theGroup, List * pathList) {

    Group* group = (Group*)theGroup;

    ListIterator pathIter = createIterator(group->paths);

    void * elem;

    while((elem = nextElement(&pathIter)) != NULL) {

        insertBack(pathList, elem);

    }

    ListIterator groupIter = createIterator(group->groups);

    void * newGroup;

    while((newGroup = nextElement(&groupIter)) != NULL) {

        getGroupPaths((Group*)newGroup, pathList);

    }

}
List* getPaths(SVGimage* img) {

    if (img == NULL) {

        return NULL;

    }

    List* pathList = initializeList(pathToString, noDelete, comparePaths);

    ListIterator iter = createIterator(img->paths);

    void * elem;

    while((elem = nextElement(&iter)) != NULL) {

        insertBack(pathList, elem);

    }

    iter = createIterator(img->groups);

    void * group;

    while((group = nextElement(&iter)) != NULL) {

        getGroupPaths(group, pathList);

    }

    return pathList;

}

int numRectsWithArea(SVGimage* img, float area) {

    if (img == NULL) {

        return 0;

    }

    if (area < 0) {

        return 0;

    }

    int count = 0;

    List* rectList = getRects(img);

    ListIterator iter = createIterator(rectList);

    void * elem;

    while((elem = nextElement(&iter)) != NULL) {

        Rectangle * temp = (Rectangle*)elem;

        //printf("target: %f\n", ceil(area));
        //printf("area of rect: %f\n", ceil(temp->width * temp->height));

        if (ceil(temp->width * temp->height) == ceil(area)) {

            count++;

        }

    }

    freeList(rectList);


    return count;

}


int numCirclesWithArea(SVGimage* img, float area) {

    if (img == NULL) {

        return 0;

    }

    if (area < 0) {

        return 0;

    }

    int count = 0;

    List* circleList = getCircles(img);

    ListIterator iter = createIterator(circleList);

    void * elem;

    while((elem = nextElement(&iter)) != NULL) {

        Circle * temp = (Circle*)elem;
        //printf("in svg\n");
        //printf("target: %lf\n", ceil(area));
        //printf("area of circle: %lf\n", ceil(temp->r * temp->r * 3.14159));

        if (ceil(temp->r * temp->r * 3.14159) == ceil(area)) {

            count++;

        }

    }

    freeList(circleList);


    return count;

}


int numPathsWithdata(SVGimage* img, char* data) {

    if (img == NULL) {

        return 0;

    }

    if (data == NULL) {

        return 0;

    }

    int count = 0;

    List* pathList = getPaths(img);

    ListIterator iter = createIterator(pathList);

    void * elem;

    while((elem = nextElement(&iter)) != NULL) {

        Path * temp = (Path*)elem;

        //printf("target: %s\n", data);
        //printf("data in path: %s\n", temp->data);

        if (strcmp(temp->data, data) == 0) {

            count++;

        }

    }

    freeList(pathList);


    return count;



}

void findLenOfGroup(Group * group, int * count, int len) {

    if (group == NULL) {

        return;

    }

    int groupLength = 0;

    groupLength += getLength(group->rectangles);
    groupLength += getLength(group->circles);
    groupLength += getLength(group->paths);
    groupLength += getLength(group->groups);

    if (groupLength == len)
        (*count)++;

    ListIterator iter = createIterator(group->groups);

    void * iterGroup;

    while((iterGroup = nextElement(&iter)) != NULL) {

        findLenOfGroup(iterGroup, count, len);

    }
}

int numGroupsWithLen(SVGimage* img, int len) {
    
    if (img == NULL) {

        return 0;

    }
    
    if (len < 0) {

        return 0;

    }

    int count = 0;

    ListIterator iter = createIterator(img->groups);

    void * group;

    while((group = nextElement(&iter)) != NULL) {

        findLenOfGroup(group, &count, len);

    }

    return count;

}

int numAttr(SVGimage* img) {

    if (img == NULL) {

        return 0;

    }

    int count = 0;

    count += getLength(img->otherAttributes);

    //printf("svg attr: %d\n",getLength(img->otherAttributes));

    List* rectList = getRects(img);
    
    ListIterator iter = createIterator(rectList);

    void * rects;

    while((rects = nextElement(&iter)) != NULL) {

        Rectangle* rect = (Rectangle*)rects;

        count += getLength(rect->otherAttributes);

        //printf("attr of rect: %d\n", getLength(rect->otherAttributes));

    }

    List* circList = getCircles(img);

    iter = createIterator(circList);

    void * circles;

    while((circles = nextElement(&iter)) != NULL) {

        Circle* circle = (Circle*)circles;

        count += getLength(circle->otherAttributes);

        //printf("attr of circle: %d\n", getLength(circle->otherAttributes));

    }

    List* pathList = getPaths(img);

    iter = createIterator(pathList);

    void * paths;

    while((paths = nextElement(&iter)) != NULL) {

        Path* path = (Path*)paths;

        count += getLength(path->otherAttributes);

        //printf("attr of path: %d\n", getLength(path->otherAttributes));

    }

    List* groupList = getGroups(img);
    
    iter = createIterator(groupList);

    void * groups;

    while((groups = nextElement(&iter)) != NULL) {

        Group* group = (Group*)groups;

        count += getLength(group->otherAttributes);

        //printf("attr of group: %d\n", getLength(group->otherAttributes));

    }

    freeList(rectList);
    freeList(pathList);
    freeList(circList);
    freeList(groupList);

    return count;
}

void addRectsToTree(SVGimage * img, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * rectList = img->rectangles;

    ListIterator iter = createIterator(rectList);

    void * rect;

    while((rect = nextElement(&iter)) != NULL) {

        node = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);
        Rectangle * tempRect = (Rectangle*)rect;

        char * x = floatToString(tempRect->x, tempRect->units);
        char * y = floatToString(tempRect->y, tempRect->units);
        char * width = floatToString(tempRect->width, tempRect->units);
        char * height = floatToString(tempRect->height, tempRect->units);

        xmlNewProp(node, BAD_CAST "x", BAD_CAST x);
        xmlNewProp(node, BAD_CAST "y", BAD_CAST y);
        xmlNewProp(node, BAD_CAST "width", BAD_CAST width);
        xmlNewProp(node, BAD_CAST "height", BAD_CAST height);
        
        free(x);
        free(y);
        free(width);
        free(height);
        
        ListIterator rectAttrIter = createIterator(tempRect->otherAttributes);

        void * rectAttr;

        while((rectAttr = nextElement(&rectAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)rectAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }

}

void addCircsToTree(SVGimage * img, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * circList = img->circles;

    ListIterator iter = createIterator(circList);

    void * circ;

    while((circ = nextElement(&iter)) != NULL) {

        node = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);
        Circle * tempCirc = (Circle*)circ;

        char * cx = floatToString(tempCirc->cx, tempCirc->units);
        char * cy = floatToString(tempCirc->cy, tempCirc->units);
        char * r = floatToString(tempCirc->r, tempCirc->units);

        xmlNewProp(node, BAD_CAST "cx", BAD_CAST cx);
        xmlNewProp(node, BAD_CAST "cy", BAD_CAST cy);
        xmlNewProp(node, BAD_CAST "r", BAD_CAST r);

        free(cx);
        free(cy);
        free(r);

        ListIterator circAttrIter = createIterator(tempCirc->otherAttributes);

        void * circAttr;

        while((circAttr = nextElement(&circAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)circAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }


}

void addPathsToTree(SVGimage * img, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * pathList = img->paths;

    ListIterator iter = createIterator(pathList);

    void * path;

    while((path = nextElement(&iter)) != NULL) {
        
        Path * tempPath = (Path*)path;
        node = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);

        xmlNewProp(node, BAD_CAST "d", BAD_CAST tempPath->data);

        ListIterator pathAttrIter = createIterator(tempPath->otherAttributes);

        void * pathAttr;

        while((pathAttr = nextElement(&pathAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)pathAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }


}
void addGroupsToTree(SVGimage * img, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * groupList = img->groups;

    ListIterator iter = createIterator(groupList);

    void * group;

    while((group = nextElement(&iter)) != NULL) {
        
        Group * tempGroup = (Group*)group;
        node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);

        ListIterator groupAttrIter = createIterator(tempGroup->otherAttributes);

        addGroupRectsToTree(tempGroup, node);
        addGroupCircsToTree(tempGroup, node);
        addGroupPathsToTree(tempGroup, node);
        addGroupGroupsToTree(tempGroup, node);

        void * groupAttr;

        while((groupAttr = nextElement(&groupAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)groupAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }

}

void addGroupRectsToTree(Group * group, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * rectList = group->rectangles;

    ListIterator iter = createIterator(rectList);

    void * rect;

    while((rect = nextElement(&iter)) != NULL) {

        node = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);
        Rectangle * tempRect = (Rectangle*)rect;

        char * x = floatToString(tempRect->x, tempRect->units);
        char * y = floatToString(tempRect->y, tempRect->units);
        char * width = floatToString(tempRect->width, tempRect->units);
        char * height = floatToString(tempRect->height, tempRect->units);

        xmlNewProp(node, BAD_CAST "x", BAD_CAST x);
        xmlNewProp(node, BAD_CAST "y", BAD_CAST y);
        xmlNewProp(node, BAD_CAST "width", BAD_CAST width);
        xmlNewProp(node, BAD_CAST "height", BAD_CAST height);
        
        free(x);
        free(y);
        free(width);
        free(height);
        
        ListIterator rectAttrIter = createIterator(tempRect->otherAttributes);

        void * rectAttr;

        while((rectAttr = nextElement(&rectAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)rectAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }
}
void addGroupCircsToTree(Group * group, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * circList = group->circles;

    ListIterator iter = createIterator(circList);

    void * circ;

    while((circ = nextElement(&iter)) != NULL) {

        node = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);
        Circle * tempCirc = (Circle*)circ;

        char * cx = floatToString(tempCirc->cx, tempCirc->units);
        char * cy = floatToString(tempCirc->cy, tempCirc->units);
        char * r = floatToString(tempCirc->r, tempCirc->units);

        xmlNewProp(node, BAD_CAST "cx", BAD_CAST cx);
        xmlNewProp(node, BAD_CAST "cy", BAD_CAST cy);
        xmlNewProp(node, BAD_CAST "r", BAD_CAST r);

        free(cx);
        free(cy);
        free(r);

        ListIterator circAttrIter = createIterator(tempCirc->otherAttributes);

        void * circAttr;

        while((circAttr = nextElement(&circAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)circAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }

}
void addGroupPathsToTree(Group * group, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * pathList = group->paths;

    ListIterator iter = createIterator(pathList);

    void * path;

    while((path = nextElement(&iter)) != NULL) {
        
        Path * tempPath = (Path*)path;
        node = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);

        xmlNewProp(node, BAD_CAST "d", BAD_CAST tempPath->data);

        ListIterator pathAttrIter = createIterator(tempPath->otherAttributes);

        void * pathAttr;

        while((pathAttr = nextElement(&pathAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)pathAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }

}
void addGroupGroupsToTree(Group * group, xmlNodePtr root_node) {

    xmlNodePtr node = NULL;

    List * groupList = group->groups;

    ListIterator iter = createIterator(groupList);

    void * grouploop;

    while((grouploop = nextElement(&iter)) != NULL) {
        
        Group * tempGroup = (Group*)grouploop;
        node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);

        ListIterator groupAttrIter = createIterator(tempGroup->otherAttributes);

        void * groupAttr;

        addGroupRectsToTree(tempGroup, node);
        addGroupCircsToTree(tempGroup, node);
        addGroupPathsToTree(tempGroup, node);
        addGroupGroupsToTree(tempGroup, node);

        while((groupAttr = nextElement(&groupAttrIter)) != NULL) {

            Attribute * temp = (Attribute*)groupAttr;

            xmlNewProp(node, BAD_CAST temp->name, BAD_CAST temp->value);

        }

    }

}

xmlDocPtr buildXMLtree(SVGimage * img) {


    List* rectList = getRects(img);


    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL;/* node pointers */

    LIBXML_TEST_VERSION;

    /* 
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(NULL);
    
    root_node = xmlNewNode(NULL, BAD_CAST "svg");
    xmlNsPtr ns = xmlNewNs(root_node, (xmlChar *)img->namespace, NULL);
    xmlSetNs(root_node, ns);

    xmlDocSetRootElement(doc, root_node);
    if (strlen(img->title) > 0)
        xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST img->title);
    if (strlen(img->description) > 0)
        xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST img->description);

    ListIterator iter = createIterator(img->otherAttributes);

    void * attr;

    while((attr = nextElement(&iter)) != NULL) {

        Attribute * temp = (Attribute*)attr;

        xmlNewProp(root_node, BAD_CAST temp->name, BAD_CAST temp->value);


    }

    addRectsToTree(img, root_node);
    addCircsToTree(img, root_node);
    addPathsToTree(img, root_node);
    addGroupsToTree(img, root_node);

    /* 
     * Dumping document to stdio or file
     */
    //xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);

    /*free the document */
    //xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    //return(0);

    freeList(rectList);


    return doc;

}

bool validateRects(List * rectList) {


    ListIterator Iter = createIterator(rectList);

    void * rect;

    while((rect = nextElement(&Iter)) != NULL) {

        Rectangle * temp = (Rectangle*)rect;

        if (temp->width < 0)
            return false;
        if (temp->height < 0)
            return false;
        if (temp->otherAttributes == NULL)
            return false;

        ListIterator Iter2 = createIterator(temp->otherAttributes);

        void * attr;

        while((attr = nextElement(&Iter2)) != NULL) {

            Attribute * tempAttr = (Attribute*)attr;

            if (tempAttr->name == NULL || tempAttr->value == NULL)
                return false;

        }

    }

    return true;

}
bool validateCircs(List * circList) {

    ListIterator Iter = createIterator(circList);

    void * circ;

    while((circ = nextElement(&Iter)) != NULL) {

        Circle * temp = (Circle*)circ;

        if (temp->r < 0)
            return false;
        if (temp->otherAttributes == NULL)
            return false;

        ListIterator Iter2 = createIterator(temp->otherAttributes);

        void * attr;

        while((attr = nextElement(&Iter2)) != NULL) {

            Attribute * tempAttr = (Attribute*)attr;

            if (tempAttr->name == NULL || tempAttr->value == NULL)
                return false;

        }

    }

    return true;

}
bool validatePaths(List * pathList) {

    ListIterator Iter = createIterator(pathList);

    void * path;

    while((path = nextElement(&Iter)) != NULL) {

        Path * temp = (Path*)path;

        if (temp->data == NULL)
            return false;
        if (strlen(temp->data) < 1)
            return false;
        if (temp->otherAttributes == NULL)
            return false;

        ListIterator Iter2 = createIterator(temp->otherAttributes);

        void * attr;

        while((attr = nextElement(&Iter2)) != NULL) {

            Attribute * tempAttr = (Attribute*)attr;

            if (tempAttr->name == NULL || tempAttr->value == NULL)
                return false;

        }
    }

    return true;

}
bool validateGroups(List * groupList) {

    ListIterator Iter = createIterator(groupList);

    void * group;

    while((group = nextElement(&Iter)) != NULL) {

        Group * temp = (Group*)group;

        if (temp->rectangles == NULL)
            return false;
        if (temp->circles == NULL)
            return false;
        if (temp->paths == NULL)
            return false;
        if (temp->groups == NULL)
            return false;
        if (temp->otherAttributes == NULL)
            return false;

        ListIterator Iter2 = createIterator(temp->otherAttributes);

        void * attr;

        while((attr = nextElement(&Iter2)) != NULL) {

            Attribute * tempAttr = (Attribute*)attr;

            if (tempAttr->name == NULL || tempAttr->value == NULL)
                return false;

        }

    }

    return true;

}


bool validateSVGimage(SVGimage* image, char* schemaFile) {


    if (image == NULL || schemaFile == NULL) {
        //printf("null values passed\n");
        return false;
    }



    // validate svg

    // validate 

    if (image->rectangles == NULL) {
        //printf("rectangles list is null\n");
        return false;
    }
    if (image->circles == NULL) {
        //printf("circles list is null\n");
        return false;
    }
    if (image->paths == NULL) {
        //printf("paths list is null\n");
        return false;
    }
    if (image->groups == NULL) {
        //printf("groups list is null\n");
        return false;        
    }
    if (image->otherAttributes == NULL) {
        //printf("otherAttributes list is null\n");
        return false;
    }

    ListIterator Iter = createIterator(image->otherAttributes);

    void * attr;

    while((attr = nextElement(&Iter)) != NULL) {

        Attribute * tempAttr = (Attribute*)attr;

        if (tempAttr->name == NULL || tempAttr->value == NULL) {
            //printf("an attriubte was null\n");
            return false;
        }

    }

    if (strlen(image->namespace) < 1) {
        //printf("namespace is empty");
        return false;
    }
    if (strlen(image->description) < 1 && strcmp(image->description, "") != 0) {
        //printf("description isn't initialized\n");
        return false;
    }
    if (strlen(image->title) < 1 && strcmp(image->title, "") != 0) {
        //printf("title isn't initialized\n");
        return false; 
    }

    
    List * rectList = getRects(image);
    List * circList = getCircles(image);
    List * pathList = getPaths(image);
    List * groupList = getGroups(image);

    if (!validateRects(rectList)) {
        //printf("failed to validate rects\n");
        freeList(rectList);
        freeList(circList);
        freeList(pathList);
        freeList(groupList);
        return false;
    }
    if (!validateCircs(circList)) {
        //printf("failed to validate circles\n");
        freeList(rectList);
        freeList(circList);
        freeList(pathList);
        freeList(groupList);
        return false;
    }
    if (!validatePaths(pathList)) {
        //printf("failed to validate paths\n");
        freeList(rectList);
        freeList(circList);
        freeList(pathList);
        freeList(groupList);
        return false;
    }
    if (!validateGroups(groupList)) {
        //printf("failed to validate groups\n");
        freeList(rectList);
        freeList(circList);
        freeList(pathList);
        freeList(groupList);
        return false;
    }



    if (!validateSchema(image, schemaFile)) {

        //printf("validation against schema returns false\n");
        freeList(rectList);
        freeList(circList);
        freeList(pathList);
        freeList(groupList);
        return false;

    }

    freeList(rectList);
    freeList(circList);
    freeList(pathList);
    freeList(groupList);

    return true;
    
}

bool validateSchema(SVGimage * img, char * schemaFile) {


    if (img == NULL || schemaFile == NULL) {

        //printf("svg image or schema was null\n");
        return false;

    }

    xmlDocPtr doc = buildXMLtree(img);

    if (doc == NULL) {

        //printf("failed to build xml tree\n");
        return false;

    }
    
    bool returnValue = false;
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;
    char *XSDFileName = schemaFile;

    xmlLineNumbersDefault(1);

    ctxt = xmlSchemaNewParserCtxt(XSDFileName);

    xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);


    if (doc == NULL) {
        //fprintf(stderr, "Could not parse\n");
    }
    else {
        xmlSchemaValidCtxtPtr ctxt;
        int ret;

        ctxt = xmlSchemaNewValidCtxt(schema);
        xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        ret = xmlSchemaValidateDoc(ctxt, doc);
        if (ret == 0) {
            //printf("validates\n");
            returnValue = true;
        }
        else if (ret > 0) {
            //printf("fails to validate\n");
            returnValue = false;

        }
        else {
            //printf("validation generated an internal error\n");
            returnValue = false;
        }
        xmlSchemaFreeValidCtxt(ctxt);

    }
    if(schema != NULL)
        xmlSchemaFree(schema);

    xmlSchemaCleanupTypes();
    xmlFreeDoc(doc);

    return returnValue;

}


SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {

    if (fileName == NULL || schemaFile == NULL) {

        return NULL;

    }

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    LIBXML_TEST_VERSION
    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        //printf("error: could not parse file %s\n", fileName);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    /*Initialize the SVGimage structure */


    SVGimage* mySVG = initSVG(doc);


    if (mySVG == NULL) {

        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;

    }
        
    /*Get the root element node */

    root_element = xmlDocGetRootElement(doc);

    buildSVG(root_element->children, mySVG);

    //writeSVGimage(mySVG, "testfile.svg");

    if (!validateSchema(mySVG, schemaFile)) {
        xmlFreeDoc(doc);
        xmlMemoryDump();
        xmlCleanupParser();
        return NULL;
    }
 
    /*free the document */

    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
    xmlMemoryDump();


    /*if (writeSVGimage(mySVG, "testfile.svg"))
        printf("SVGimage was written to file\n");
    else 
        printf("unable to write SVGimage to file\n");*/
    return mySVG;

}



bool writeSVGimage(SVGimage* image, char* fileName) {

    if (image == NULL || fileName == NULL)
        return false;

    bool returnValue = false;

    xmlDocPtr doc;

    if (image != NULL && fileName != NULL) {

        doc = buildXMLtree(image);
        xmlSaveFormatFileEnc(fileName, doc, NULL, 1);
        returnValue = true;

    }

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return returnValue;

}

void setSVGAttribute(SVGimage * image, Attribute* newAttribute) {

    if (strcmp("namespace", newAttribute->name) == 0) {

        //printf("addingnamespace\n");
        strncpy(image->namespace, newAttribute->value, 255);
        image->namespace[255] = '\0';
        deleteAttribute(newAttribute);
        return;
    }
    if (strcmp("title", newAttribute->name) == 0) {

        //printf("addingtitle\n");
        strncpy(image->title, newAttribute->value, 255);
        image->title[255] = '\0';
        deleteAttribute(newAttribute);
        return;
    }
    if (strcmp("description", newAttribute->name) == 0) {

        //printf("addingdescription\n");
        strncpy(image->description, newAttribute->value, 255);
        image->description[255] = '\0';
        deleteAttribute(newAttribute);
        return;
    }


    ListIterator Iter = createIterator(image->otherAttributes);

    void * attr;

    bool equal = false;

    while((attr = nextElement(&Iter)) != NULL) {

        Attribute * temp = (Attribute*)attr;
        if (strcmp(temp->name, newAttribute->name) == 0) {

            //printf("%s = %s\n", temp->name, newAttribute->name);

            free(temp->value);
            temp->value = stringdup(newAttribute->value);
            equal = true;
            deleteAttribute(newAttribute);


        }
    }

    if (!equal) {

        insertBack(image->otherAttributes, newAttribute);

    }

}

void setRectAttribute(Rectangle * rect, Attribute* newAttribute) {

    if (strcmp("x", newAttribute->name) == 0) {

        //printf("addingx\n");

        //printf("%f\n", strtof(newAttribute->value, NULL));
        //printf("%f\n", rect->x);

        rect->x = strtof(newAttribute->value, NULL);

        deleteAttribute(newAttribute);

        //printf("returning\n");
        return;
    }
    if (strcmp("y", newAttribute->name) == 0) {

        //printf("addingy\n");
        rect->y = strtof(newAttribute->value, NULL);


        deleteAttribute(newAttribute);
        return;
    }
    if (strcmp("width", newAttribute->name) == 0) {

        //printf("addingdescription\n");
        rect->width = strtof(newAttribute->value, NULL);

        deleteAttribute(newAttribute);
        return;
    }
    if (strcmp("height", newAttribute->name) == 0) {

        //printf("addingdescription\n");
        rect->height = strtof(newAttribute->value, NULL);

        deleteAttribute(newAttribute);
        return;
    }

    ListIterator Iter = createIterator(rect->otherAttributes);

    void * attr;

    bool equal = false;

    while((attr = nextElement(&Iter)) != NULL) {

        Attribute * temp = (Attribute*)attr;
        if (strcmp(temp->name, newAttribute->name) == 0) {

            //printf("%s = %s\n", temp->name, newAttribute->name);
            free(temp->value);
            temp->value = stringdup(newAttribute->value);            
            equal = true;
            deleteAttribute(newAttribute);


        }
    }

    if (!equal) {

        insertBack(rect->otherAttributes, newAttribute);

    }

}


void setCircAttribute(Circle * circ, Attribute* newAttribute) {

    if (strcmp("cx", newAttribute->name) == 0) {

        //printf("addingcx\n");
        circ->cx = strtof(newAttribute->value, NULL);
        deleteAttribute(newAttribute);
        //printf("returning\n");
        return;
    }
    if (strcmp("cy", newAttribute->name) == 0) {

        //printf("addingcy\n");
        circ->cy = strtof(newAttribute->value, NULL);

        deleteAttribute(newAttribute);
        return;
    }
    if (strcmp("r", newAttribute->name) == 0) {

        //printf("addingdescription\n");
        circ->r = strtof(newAttribute->value, NULL);

        deleteAttribute(newAttribute);
        return;
    }


    ListIterator Iter = createIterator(circ->otherAttributes);

    void * attr;

    bool equal = false;

    while((attr = nextElement(&Iter)) != NULL) {

        Attribute * temp = (Attribute*)attr;
        if (strcmp(temp->name, newAttribute->name) == 0) {

            //printf("%s = %s\n", temp->name, newAttribute->name);
            free(temp->value);
            temp->value = stringdup(newAttribute->value);            
            equal = true;
            deleteAttribute(newAttribute);

        }
    }

    if (!equal) {

        insertBack(circ->otherAttributes, newAttribute);

    }

}

void setPathAttribute(Path * path, Attribute* newAttribute) {

    if (strcmp("d", newAttribute->name) == 0) {

        path->data = realloc(path->data, strlen(newAttribute->value) + 1);
            
        strcpy(path->data, newAttribute->value); 

        deleteAttribute(newAttribute);

        return;
    }


    ListIterator Iter = createIterator(path->otherAttributes);

    void * attr;

    bool equal = false;

    while((attr = nextElement(&Iter)) != NULL) {

        Attribute * temp = (Attribute*)attr;
        if (strcmp(temp->name, newAttribute->name) == 0) {

            //printf("%s = %s\n", temp->name, newAttribute->name);
            free(temp->value);
            temp->value = stringdup(newAttribute->value);
            equal = true;
            deleteAttribute(newAttribute);

        }
    }

    if (!equal) {

        insertBack(path->otherAttributes, newAttribute);

    }

}

void setGroupAttribute(Group * group, Attribute* newAttribute) {

    if ((newAttribute == NULL || newAttribute->name == NULL ) || (newAttribute->value == NULL || group == NULL)) {
        deleteAttribute(newAttribute);
        return;
    }

    ListIterator Iter = createIterator(group->otherAttributes);

    void * attr;

    bool equal = false;

    while((attr = nextElement(&Iter)) != NULL) {

        Attribute * temp = (Attribute*)attr;

        if (temp->name == NULL || temp->value == NULL) {

            deleteAttribute(newAttribute);
            return;

        }

        if (strcmp(temp->name, newAttribute->name) == 0) {

            //printf("%s = %s\n", temp->name, newAttribute->name);
            free(temp->value);
            temp->value = stringdup(newAttribute->value);
            equal = true;
            deleteAttribute(newAttribute);
            return;

        }
    }

    if (!equal) {

        insertBack(group->otherAttributes, newAttribute);

    }

}

void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute) {

    if (image == NULL || newAttribute == NULL) {

        return;

    }

    if (newAttribute->name == NULL || newAttribute->value == NULL) {

        return;

    }

    if (elemType == SVG_IMAGE) {

        //printf("set svg attr\n");
        setSVGAttribute(image, newAttribute);

    }
    if (elemType == RECT) {

        //printf("set rect attr\n");

        if ((getLength(image->rectangles)-1) < elemIndex || elemIndex < 0) {

            //printf("no rect\n");
            deleteAttribute(newAttribute);
            return;

        }

        ListIterator Iter = createIterator(image->rectangles);

        void * rect;

        rect = nextElement(&Iter);

        for (int i = 0; i < elemIndex; i++) {

            rect = nextElement(&Iter);

        }

        printf("%s\n", rectangleToString(rect));

        setRectAttribute(rect, newAttribute);   

        printf("after:\n %s\n", rectangleToString(rect));     


    }
    if (elemType == CIRC) {

        //printf("set circ attr\n");

        if ((getLength(image->circles)-1) < elemIndex || elemIndex < 0) {

            //printf("no circ\n");
            deleteAttribute(newAttribute);
            return;

        }

        ListIterator Iter = createIterator(image->circles);

        void * circ;

        circ = nextElement(&Iter);

        for (int i = 0; i < elemIndex; i++) {

            circ = nextElement(&Iter);

        }

        printf("%s\n", circleToString(circ));

        setCircAttribute(circ, newAttribute);   

        printf("after:\n%s\n", circleToString(circ));

    }
    if (elemType == PATH) {

        //printf("set path attr\n");

        if ((getLength(image->paths)-1) < elemIndex || elemIndex < 0) {

            //printf("no path\n");
            deleteAttribute(newAttribute);
            return;

        }

        ListIterator Iter = createIterator(image->paths);

        void * path;

        path = nextElement(&Iter);

        for (int i = 0; i < elemIndex; i++) {

            path = nextElement(&Iter);

        }

        printf("%s\n", pathToString(path));

        setPathAttribute(path, newAttribute); 

        printf("AFTER:\n");

        printf("%s\n", pathToString(path));
  

    }
    if (elemType == GROUP) {

        //printf("set group attr\n");

        if ((getLength(image->groups)-1) < elemIndex || elemIndex < 0) {

            //printf("no group\n");
            deleteAttribute(newAttribute);
            return;

        }

        ListIterator Iter = createIterator(image->groups);

        void * group;

        group = nextElement(&Iter);

        for (int i = 0; i < elemIndex; i++) {

            group = nextElement(&Iter);

        }

        setGroupAttribute(group, newAttribute);   

    }

}



void addComponent(SVGimage* image, elementType type, void* newElement) {

    if (image == NULL || newElement == NULL) 
        return;

    if (type == SVG_IMAGE) 
        return;

    
    if (type == RECT) {

        insertBack(image->rectangles, newElement);

    }
    if (type == CIRC) {

        insertBack(image->circles, newElement);

    }
    if (type == PATH) {

        insertBack(image->paths, newElement);

    }
    if (type == GROUP) {

        insertBack(image->groups, newElement);

    }


}



char* attrToJSON(const Attribute *a) {

    char * emptyString = malloc(sizeof(char)*4);

    strcpy(emptyString, "{}");

    if (a == NULL) 
        return emptyString;
    
    if (a->name == NULL)
        return emptyString;

    if (a->value == NULL)
        return emptyString;

    free(emptyString);

    int stringLength = 0;

    stringLength = strlen("{\"name\":\"")+strlen(a->name) + strlen("\",\"value\":\"") + strlen(a->value) + strlen("\"}");

    char * returnString;

    returnString = malloc(stringLength+1);

    sprintf(returnString, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);

    return returnString;

}
char* circleToJSON(const Circle *c) {

    char * emptyString = malloc(3);

    strcpy(emptyString, "{}");

    if (c == NULL) 
        return emptyString;

    free(emptyString);

    int stringLength = 0;

    char* returnString;
    char* attrString = intToString(getLength(c->otherAttributes), NULL);
    char* cxString = floatToString(c->cx, NULL);
    char* cyString = floatToString(c->cy, NULL);
    char* rString = 
/*****  VA_Node_Entity Implementation  *****/

/************************
 ************************
 *****  Interfaces  *****
 ************************
 ************************/

/********************
 *****  System  *****
 ********************/

#include "Vk.h"

/******************
 *****  Self  *****
 ******************/

#include "va_node_entity.h"

/************************
 *****  Supporting  *****
 ************************/

#include <iostream>


/******************************
 ******************************
 *****                    *****
 *****  VA::Node::Entity  *****
 *****                    *****
 ******************************
 ******************************/

/**************************
 **************************
 *****  Construction  *****
 **************************
 **************************/

VA::Node::Entity::Entity () {
}

/*************************
 *************************
 *****  Destruction  *****
 *************************
 *************************/

VA::Node::Entity::~Entity () {
}


/***************************
 ***************************
 *****  Class Builder  *****
 ***************************
 ***************************/

VA::Node::Entity::ClassBuilder::ClassBuilder (Vxa::VClass *pClass) : BaseClass::ClassBuilder (pClass) {
    defineConstant (".apiGeneration", static_cast<int>(VA::Node::VisionAPI_Generation));
}

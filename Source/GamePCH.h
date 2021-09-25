#pragma once

// Precompiled Header

/*
	New
	Locates and reserves storage for the object or objects to be allocated. When this stage is complete, the correct amount of storage is allocated, but it is not yet an object.

	Initializes the object(s). Once initialization is complete, enough information is present for the allocated storage to be an object.

	Returns a pointer to the object(s) of a pointer type derived from new-type-name or type-name. The program uses this pointer to access the newly allocated object.

	new is an operator whereas malloc() is a library function.
	new allocates memory and calls constructor for object initialization. But malloc() allocates memory and does not call constructor.
	Return type of new is exact data type while malloc() returns void*.
	new is faster than malloc() because an operator is always faster than a function
	new throws on failure, malloc returns NULL.
* 
* 
*/

// using do while that doesn't loop to prevent errors when calling SafeDelete inside an if-else statement
#define SafeDelete(p)		 do{ if(p != nullptr) {delete p; p = nullptr;} }while(0)
#define SafeDeleteArray(p)   do{if(p != nullptr){delete[] p; p = nullptr;} }while(0)

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio.hpp"

#include "Math.h"
#include "LineSegment.h"
#include "Easing.h"
#include "Tween.h"
#include "Constants.h"

#include <Windows.h>
#include <windowsx.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>

#include <fstream>  
#include <iostream>
#include <sstream>

#include <map>
#include <vector>
#include <string>
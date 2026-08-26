# Voxel Engine
---

Prior to reading this file, make sure you have taken notice of the project's [license](https://github.com/Allosker/voxel_engine/blob/main/license.txt " read carefully ").

--- 

## Conventions

Do not use conventional types, every "natural" type has been redefined in the "types.hpp" file, and it must be used at all times in place of "normal" types, also, it defines plenty of type alliases for commong matrices/vectors/quaternions/angles.


Class/Struct/Union names follow the Camel Case convention: "MyType".
Variables names use the snake case: "my_var"
Function names use the snake case convention, apart from, predicares, getters and setters.

Getters/Setters are lowercase for their first word, that is "set" or "get", but the following names follows the Domedary Convention: "get_myVar", "set_myVar".
Predicates use the Dromedary Convention: "isEmpty", "hasValue", "wasUpdatedLastFrame".

File names use the Dromedary convention: "chunkMesh", "inputManager".
Namespaces are to be a single word (preferably an abbreviation for it).


I use the extensions: .hpp/.cpp for C++, and .h/.c for C.
For the shaders, I use:

.vert/.frag/.geom (I don't think you'll need any such shaders but I prefer to be clear about it).

Also, I use "#pragma once", and I prefer not to have transively included classes/functions for .hpp files. 
You do not need to #include everything that was included in the .hpp file of a .cpp file, this is allowed, but prefer not to include anything in the .hpp file if it is only used in the .cpp file.

Member variables: 


private member variables start with this: m_
global constexpr variables start with: g_


```cpp
void foo() noexcept
{
  for (i32 i{}; i < 10; i++)
  {
  }
}

class MyClass
{
public:
protected:
  // code

  void get_this() noexcept;
  void get_that() noexcept;

  void set_this() noexcept;

  void set_that() noexcept;


private:

  i8 m_var;
  
  i16 m_other;


}
```

I usually separate every section with a double new line, I call a section things that are related: 

Like, if a bunch of functions are for adding/removing stuff for the invnetory, I put them together, if another set of functions is there for sorting the invneotry out, I consider that another section.
Also, functions can stay in the .hpp file if they don't exceed 3/4ish lines.

I usually separate every section with a double new line, I call a section things that are related: 

Like, if a bunch of functions are for adding/removing stuff for the invnetory, I put them together, if another set of functions is there for sorting the invneotry out, I consider that another section.
Also, functions can stay in the .hpp file if they don't exceed 3/4ish lines.

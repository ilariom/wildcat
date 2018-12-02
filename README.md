# Wildcat
Wildcat is a 2D game engine based on SDL2 built from these design principles:
* **Modern C++**, both for engine implementation that for client programming
* Type-safe **Entity Component System**
* **Hybrid hardware / software rendering** with a bias towards software
* Software shading and image processing through **pixel manipulators**
* 2D-only
* **High portability of code**. It depends only on SDL2 and standard C++
* No garbage collection or custom solutions for memory management. Instead it uses modern C++ techniques such as **resource handles** to clarify ownership and lifecycle of objects
* **Lightweight** core architecture and **highly extensibile**
* **Zero-overhead principle**: only what you use consume resources

### Introduction
While being still deep into development, the purpose of this document is to illustrate already available functionalities and bits of the future of the project.
As with many modern game engines, at the core of *Wildcat* lies an Entity Component System. The traditional approach to ECS is to have small Entity objects (typically **very** small) that identify *things* inside the game, detached components aggregated together that maintain specific informations about the entities, represented by simple structures (PODs are very common), and systems that realize behaviour.
*Wildcat* approach instead is to have entities as real object that handle their components, components of arbitrary complexity that can represent both data and logic, and systems that are used to apply some behaviour to a collection of components.
### World, Scene and SceneGraph
The objects with the wider view are the ones that know about entities, components and systems. These objects may apply systems behaviour on the right components of the entities. Given that, they are called **ECSContext**s.
Concrete **ECSContext**s are:
* **World**, a singleton object that lives throughout the game, 
* **Scene**, an object that represent big chunks of the game, and
* **SceneGraph**, objects that represent a hierarchical view of the **Scene**. This is the only **ECSContext** that handles a **Camera** and provide a **RenderSystem**.

**World** and **Scene**s are completely independent, while **SceneGraph**s must be attached to a **Scene** to make sense. More than one **SceneGraph** may be added, simplifying, for example, the separation between the *"real"* action of the game from the UI. Each **Scene** has a default **SceneGraph** that can be retrieved by a call to ```wkt::scene::getCurrentScene().getDefaultSceneGraph()```.
The namespace ```wkt``` is where all the engine code reside. In this example the **SceneGraph** of the current scene is retrieved.
### Entities, components and systems
Entities are instances of the ```wkt::ecs::Entity``` class, that assigns automatically a unique identifier to the object and manage its components. Components are derived classes of ```wkt::ecs::Component```. A component must be a copiable and moveable object registered to the engine with the macro ```REGISTER_COMPONENT(<type>, <number_id>)```. This macro is required as it will make some compile-time magic to allow type safety through internally generated type traits. Unfortunately, the C++ standard doesn't specify a way to have compile-time counters, so the *<number_id>* must be placed manually when a new component is built. Compilers often provide ```__COUNTER__``` to do that. Some libraries, such as Boost, have their own, so you should not have troubles handling that. However, as for design principle, the engine can't do that by itself. 
Note that negative IDs are reserved.
Systems are derived classes of ```wkt::ecs::System```. Looking at the source code, their implementation may seem a bit too much obscure, but the main point is that they exist in two kinds:
* **Sequential systems**, that apply their behaviour to components through a collection of entities
* **Hierarchical systems**, the ones that run through a tree of nodes. These are typically used when update transforms or for rendering.

Note that, as for the *zero-overhead principle*, systems must be attached explicitly to their **ECSContext** to run. Some components may require the existence of a system to do something useful.
Putting all together, an example of use may look like this:
```
    // Build a scene
    auto scene = std::make_shared<wkt::scene::Scene>();
    // Ask for an entity from the ECSContext. Entities lifecycle depends always from the EntityManager of the context.
    auto& entity = scene->getDefaultSceneGraph().entityManager().make();
    // Make some components. Note the use of shared pointers
    auto node = std::make_shared<Node>();
    auto mt = std::make_shared<Transform>();
    // Attach components to the entity
    entity += node;
    entity += mt;
    entity += std::make_shared<MouseReceiver>();
    // MouseReceiver requires a system to make sense. Add that to the context
    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::MouseReceiverSystem>();
    // Finally, run the scene
    wkt::scene::runScene(scene);
```
By looking at this snippet, we can see that the context (here, a **Scene**) manages entities and systems through the specialized managers **EntityManager** and **SystemsManager**. We can request a new entity via a ```make()``` call and destroy it through the function ```kill()``` of the manager. An entity may be copied and the copy added to another manager that can gain ownership on it through ```gain()```. You can't create an Entity using a constructor.
Each component has a ```getEntity()``` function that returns the entity that owns it, if any. Querying components is as simple as a call to ```query<COMPONENT_TYPE>()```. Since some components of the same kind can coexist in the same entities (something called uniqueness of the component), the result of query is a vector of components of the type requested. To simplify programming, it casts to bool automatically (to test existence) and overloads ```operator*``` to return the first element.
So, for example, we may want to do something like that:
```
auto transforms = entity.query<Transform>();
if(transforms)
    auto t = *transforms; // do something with 't', the entity transform
...
// A terse way to retrieve a sprite, when we know that it surely exists
auto sprite = *entity.query<Sprite>();
```
### Built-in components
While being an ECS means that programmers should be able to make their own components and systems, one of the goals of *Wildcat* is to reduce the effort by providing a collection of built-in components to kickstart productivity without bothering with some of the lower level details.
##### Nodes and transforms
Nodes are the basic building block of the scene graphs. They expose a straightforward interface typical of the component with ```appendChild()```, ```removeChild()```, ```getParent()``` functions and others that you may expect. A special mention to the ```prune()``` function that prevents hierarchical systems to go deep down the hierarchy when is needed to.
Transforms are straightforward too. They hold all the geometrical information of the entity, except its size, and retain both local and world coordinates to be accessed.
##### Data stores
These are generic components that hold arbitrary information. They may be useful for exchanging data between components that implements logic, or as a common, single source of data attached to some context.
More specifically, as of now, we have **Dictionary** as key-value pairs of templated types, **JSON** that implements the well-known format to represent objects and **Table**s that are database-like structures that can be queried. They are represented by a vector of tuples, to be consistent with the design principle of using modern C++ features.
##### Input and scripts
By now, *Wildcat* supports only mouse and raw keyboard inputs. More effort will be done in this direction, so these features should grow dramatically in the future. Events like that are managed by *receiver* components and their coupled systems. **Action**s are a kind of virtual input based on messages passed through the entities.
**Script**s are generic controllers loosely based on the ones with the same name that you may find in *Defold*. They have an ```update()``` function and the helper functions to control scheduling, an ```init()``` function called only once at object initialization, and an internal messaging system with fine-grained control to whom should receive the message. As for the *zero-overhead principle*, the last feature require a system of its own to be explicitly attached to the context.
##### Drawables
These components are the ones that allow you to make something on the screen. There is a **Text** component that supports only TTFs for now, good old **Sprite**s to draw images, and **Crowd**s. These are collection of **[Sprite**, **Transform]** pairs (called **Spectator**s) useful for composing elements from multiple images.
### Pixel manipulators
Some of the drawable elements can be accessed on a per-pixel base to modify the way they look. **Pixel manipulator**s works similarly as shaders, with the important exception that they process images via CPU rather than GPU. This is the core concept of the software-side renderer of the engine. Their main advantages are:
* Portability and efficiency, as they are written in C++ and compiled native
* No bandwidth problems, as they not work through I/O
* Highly expressive, as no restrictions apply to the possible computations
* Easy debugging through traditional instruments
* Multi-pass shading is simple
* Cached results, so you need to perform computations only once, instead of every frame

Obviously enough, hardware shaders work better in many cases, but efforts are made to allow the use pixel manipulators minimizing performance penalties. And soon many other optimizations and extensions to this mechanism will be done, as they are central to the project.
### Coming soon
* Event-based audio engine (which is already there, it just needs some tweaks)
* Bitmap fonts
* Texture atlases
* Simplified, flipbook and skeletal animations
* Game controllers and touchscreen support
* Mobile platforms specific support
* Pixel manipulation of slices, chaining and composition, parallel computations
* Multithreaded based optimizations

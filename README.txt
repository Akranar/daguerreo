
This project depends on CMake to generate build files. 
    -> Use this command to generate files to build Daguerreo for 32-bit using OpenGL 2.1:
        cmake -DUSE_OPENGL21=ON -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 <src>
            -> where <src> is the Daguerreo src directory.


Basic Architecture Overview:										
	The Daguerreo Rendering Library is separated into many modules, which will be referred as Components.									
		-> Each Component follows the naming convention: <LibraryName>_<ComponentName>								
                where LibraryName is the name of the Library in which Component belongs.							
                and where ComponentName is the name of the Component.							
			-> Example: Graphics_RendererBase is the RendererBase Component that belongs to the Graphics library.							
		-> Each Component is usually made of a collection of classes 
                that work together to implement a certain concept.								
			-> Example: the Component, Geometry_QuickHull, implements the quick hull algorithm 
                used to calculate the convex hull of a set of points.							
		-> Each Component may have other Components as dependencies 
                and this is signified by the presence of a file that possesses the "ExLib" prefix.								
			-> Example: the Geometry_QuickHull component depends on the Geometry_Topology component 
                    and this is apparent by the presence of a file named "ExLib_Topology.h" 
                        in the Geometry_QuickHull folder.							
				-> Every file in Geometry_QuickHull that includes files from Geometry_Topology 
                    must do so by including the file "ExLib_Topology.h" 
                       instead of including the specific individual files in Geometry_Topology.						
			-> Besides Components referencing other Components through "ExLib" files, 
                    every Component can also reference files in the Core_Include folder.  							
				-> The Core_Include is the only folder in the project that is set in the include path.						
					-> Thus, every Component has an implicit dependency on the Core_Include folder.					
				-> Components in the Core_Include folder are required to have zero dependencies 
                        other than the standard libraries 
                        and must consist of at most one header file and one source file.						
			-> This system allows a developer to easily see the interconnectivity of each Component 
                    by the presence of ExLib files 							
				-> and allows one to extract a subset of the Components from the main project 
                    without having to search through the individual files for dependencies.						
			-> Each ExLib file is a file whose only purpose 
                    is to include the *_LIB.h file inside the referenced Component.							
				-> The *_LIB.h file in a Component is a file whose only purpose 
                    is to include and expose all the relevant files 
                    containing the interfaces for the Component.						

Components:
    Core_Include				
        -> Includes core Components that are too small to warrant linking them separately 
                as statically linked libraries.	
            -> Components included in this folder are required to have zero dependencies 
                other than the standard libraries and must 
                consist of at most one header file and one source file.
                        
        Core_Assert
            -> Custom function for assertion.
                Prints a user-specified messsage to standard output upon triggering assertion.	
        Core_AsynchronousFile
            -> Class providing an interface for Asynchronous IO.  
                Currently only implemented for Win32 Overlapped File IO.	
        Core_AutoPointer
            -> A smart pointer that signifies ownership of the pointed object.	
        Core_Bitfield
            -> Class for a bitfield.  
                Provides an interface for accessing individual bits of an integer type.	
        Core_ByteArray
            -> Class representing an array of bytes.	
        Core_cstr		
            -> Routines for working with c-style strings.  
                Currently only has a function for case-insensitive string comparison.	
        Core_DoubleTimer			
            -> A class encapulating operating-system calls for high-precision time queries.	
        Core_EndianAbstractor			
            -> Abstraction used for swizzling byte-order if a difference in endianness is detected.	
        Core_Exception			
            -> A basic exception object that contains a string specifying the reason the exception was thrown.	
        Core_FastMutex
            -> Third Party Library: Taken from TinyThread++ for fast mutex locking.	
        Core_FixedArray	
            -> An implementation of an array similar to std::vector but is not resizable. 
                Accepts the use of custom allocators.	
        Core_MemoryMonitor	
            -> Singleton class that monitors the usage of C++ operators new and delete, 
                reporting any memory leaks between specified critical sections.  
                Uses simple mutex locking for synchronization between threads.	
        Core_ObjectFactory	
            -> A simple object factory.  
                Mainly used by Core_Serialization to unserialize polymorphic objects.	
        Core_pstdint			
            -> A header file declaring portable integer types.	
        Core_RandGen
            -> Simple class encapulating the generation of 
                seeded random integers, floats, and doubles using the standard c library.	
        Core_RefCountPointer
            -> A smart pointer that implements reference counting.	
        Core_RTTI
            -> Class for storing Runtime Type Information.  Currently not used.	
        Core_ScopedThread
            -> A thread object that, in the event that the object goes out of scope, 
                blocks until the corresponding thread terminates.	
        Core_Semaphore
            -> Semaphore synchronization object.  
                Currently only implemented for Win32.	
        Core_StaticAssert
            -> Template for static compile time checking.  
                Concept taken from Loki C++ Library.	
        Core_StaticTypeCheck
            -> Template for compile time type checking of pointers.  
                Concept taken from Loki C++ Library.	
        Core_SynchronousFileIO
            -> Simple class for binary file input/output.	
        Core_TinyThread	
            -> Third Party Library for thread class and common synchronization objects.	
        Core_TypeEmpty	
            -> An empty type that can be inherited from. Concept taken from Loki C++ Library.	
        Core_TypeFromInt
            -> Template to produce a type from an integer. Concept taken from Loki C++ Library.	
        Core_TypeFromType
            -> Template to produce a type from a type. Concept taken from Loki C++ Library.	
        Core_TypeList
            -> A list of types.  Used in template meta-programming. Concept taken from Loki C++ Library.	
        Core_TypeNull
            -> A class representing a Null type.  
                Typically used at the end of a type-list to signify the end of the list.
                Concept taken from Loki C++ Library.	
        Core_TypeSelect
            -> A template that selects one of two types based on a boolean value.  
                Concept taken from Loki C++ Library.	

    Algorithms_NTree
        -> Provides routines for implementing tree algorithms.  
            Currently only used by SceneGraph_SceneBase for tree recursion routines.
    Animation_SkeletalAnimation	
        -> Provides data-structures for storing animation data used to update the poses of skinned meshes.
    Assets_Assimp	
        -> Third Party Library used for importing mesh and scene data of various formats.
    Assets_Converter				
        -> Converts scene data read by Assimp to engine specific binary formats.
    Core_FileSystem				
        -> Encapulate operating system calls for working with file system directories.  
            Currently only used to set the root Resource directory identically across operating systems.
    Core_FloatDataAccessor				
        -> Provides an abstraction layer for accessing arrays of floating-point data.  
            Mainly used by routines in Geometry_BoundCreation and Geometry_QuickHull.
    Core_MemoryAllocators				
        -> Custom memory allocation strategies: 
            -> Stack based allocation, 
            -> fixed-sized pool allocation, 
            -> byte-aligned allocation.
    Core_Serialization				
        -> Provides an interface for object serialization and unserlialization.
    Core_WindowEvents				
        -> Encapulates operating system specific calls used for the creation and management of windows.
    Geometry_BoundCreation				
        -> Routines for creating bounding volumes from sets of points provided by Core_FloatDataAccessor.
    Geometry_BoundingVolume	
        -> Definitions for Bounding Volumes: AABB, OBB, BoundingSphere
    Geometry_GeometricPrimitives
        -> Definitions for various basic geometric shapes: 
            -> Cone, 
            -> Cylinder, 
            -> Line, 
            -> LineSegment, 
            -> Plane, 
            -> Polygon, 
            -> Polyhedron, 
            -> Quadrilateral, 
            -> Ray, 
            -> Rectangle3D, 
            -> Tetrahedron, 
            -> Triangle.
    Geometry_GeometricTests		
        -> Collection of geometric tests including intersection tests between primitives.
    Geometry_ProceduralMesh	
        -> Routines to procedurally generate meshes.  Torus, Box, Terrain Patch.
    Geometry_QuickHull	
        -> Implementation of QuickHull algorithm.
    Geometry_Topology	
        -> Data-structures for adjacency information: AdjacencyMatrix, AdjacencyList, WingedTriangle
    Graphics_API_OpenGL	
        -> Include Library for OpenGL function declarations. GLEW.
    Graphics_Projector	
        -> Math for working with projectors (usually cameras).  
            Includes View and Projection Matrix and Frustum plane calculations.
    Graphics_RendererBase
        -> Low level Renderer. Wraps Graphics APIs OpenGL and Direct3D.
    Graphics_RendererFX	
        -> An Effects Framework (similar to CgFX and Direct3D's FX) 
            for encapsulating Render States and Resources with Shaders.
    Image_LibPNG154	
        -> Third Party Library: LibPNG version 154. 
            Used for in-memory encoding and decoding of PNG images.
    Image_ProceduralTexture	
        -> Routines to procedurally generate simple textures for testing.
    IO_BinaryFileStream	
        -> A stream representing a binary file.  
            Used with objects implementing the Serializable interface.
    IO_SOIL	
        -> A third party library: Simple OpenGL Image Library.  
            Currently used for loading image data.
    IO_TextureLoader	
        -> Routines responsible for loading image data as texture resources for RendererBase.  
            -> Currently uses SOIL.
    IO_XMLParser	
        -> A lightweight standalone XML parser.  
            -> Currently only supports ASCII character set.
    IO_ZipArchiver	
        -> Responsible for browsing and extracting file information from Zip files.
    IO_ZLIB		
        -> Third party library.  
        -> Used by LibPNG for decoding (INFLATE) and encoding (DEFLATE) PNG image files.  
        -> Also used with IO_ZipArchiver to extract files from zip archives.
    Registry_Catalogs	
        -> Catalogs implementing various resource management policies such as reference counting.
    Registry_GraphicsRegistryBase	
        -> Registry layer responsible for managing low level rendering resources for use with RendererBase.  
            -> Default behavior uses reference counting catalogs for each resource type.
    Registry_GraphicsRegistryFX	
        -> Registry layer responsible for managing FX structures for use with Graphics_RendererFX framework.
    Registry_SceneRegistryBase
        -> Registry layer responsible for managing SceneGraph nodes.
    SceneGraph_RendererSG	
        -> Renderer layer for rendering SceneGraph nodes.
    SceneGraph_SceneBase
        -> Base layer of scene graph library.  
        -> Contains the core node types used in the scene graph: Spatial, SceneNode, Visual
    SceneGraph_Skeleton	
        -> Contains the Skeleton, a hierarchy of joints, 
            and the SkinnedMesh class which holds a reference to Skeleton.


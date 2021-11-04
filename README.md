# OpenGL-Object-Loading
OpenGL Object Loading can load virtually every 3d.obj file you can find on the internet, without using another object loading library (assimp for example). The program can load Object with 12M+ triangles and more.

The purpose of this project is to provide a shooting point for my camera in the physical based rendering with Lux project. Real time camera position can be turn on to choose the best view spot. 

# Challange
Loading object in opengl seem to be an easy problem at first, but it acttually can be very hard. The following problems will arise when you tried to load an object without using well known library like assimp:

1. Apply texture is non trivial in opengl:
 
If you only draw object by vertex coordinate, that's easy. But apply texture can become a problem, texture will become disarray and there seem to be no way to fix. This problem arise because one vertex share many texture coordiante, and opengl only store the first texture coordinate you fetch into memory. 
To Solve this problem, treat them as seperate vertex, for example, if v have 2 texcoords: t1, t2 and you have to store them as v-t1, v-t2, instead of v-t1-t2.

2. Limited Texture available:

OpenGL only allowed maxium 32 textures at a times while Crytek-Sponza have over 70 textures, San-Miguel have 324 textures. To overcome this problem, model are drawn by mesh instead of drawing as a whole, this technique also explain why assimp use mesh in their source code.

Fun Fact: Most Tutorial on the internet, if not all, choose the easy way, they only show you how to load object having 1 or 2 textures and not telling you how to do it when there are more than 32 textures.

3. Model which is too large:

If you want draw large model like San-Miguel 10M triangles, Factory 12M triangles and bigger. Computer usually ran out of memory and return error. There are two reason this problem occur:

a. You tried to draw the model as "a Whole", you have to divide model into mesh, the most optimal way is to split them by material.

b. You stored VAO, VBO and IBO into one big array or vector. Again, divide them into mesh, each mesh have its own data.

Above is the 3 main problems when it come to loading object, after completely understand the source code, you will have a more in depth look of how object loading library work and the way they run millions of triangles in real time! 

Noted: it is only possible to load SanMiguel with modern OpenGL pipeline, old OpenGL will crush your computer without mercy!

# Gallerry

San-Miguel 10M Triangles.
![San_Miguel](https://user-images.githubusercontent.com/93391908/139820994-3ecf0b20-9775-4fa5-8cef-04062b3b7506.png)

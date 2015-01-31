#include<bits/stdc++.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <cstring>
#include<SOIL/SOIL.h>
//Custom Libraries
//#include "../libs/obj_loader.cpp"
#include "tiny_obj_loader.h"

using namespace std;

GLfloat gol = 0.0f;

class Model_OBJ{
	public:
	Model_OBJ()
	{		
	}
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	map<string,GLuint> texmaps;
	string mtlpath;

	bool LoadTextures()
	{
		tinyobj::material_t mat;
		string path;
		for(unsigned int i =0 ; i<shapes.size();++i)
		{
			if(!shapes[i].mesh.material_ids.empty())
			{
				mat = materials[shapes[i].mesh.material_ids[0]];	//assuming one material per shape
				if(!mat.diffuse_texname.empty())
				{
					cout<<"Loading texture: "<<mat.diffuse_texname<<endl;
					path = mtlpath+mat.diffuse_texname;
					GLuint tex = SOIL_load_OGL_texture(
						path.c_str(),
						SOIL_LOAD_AUTO,
						SOIL_CREATE_NEW_ID,
						SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
					);
					cout<<"Loaded"<<endl;
					if( 0 == tex )
					{	
						printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
					}

					texmaps[mat.name] = tex;
				}
			}
		}
		return true;
	}

	bool Load(const char* filename, const char* basepath = NULL)
	{
	  std::cout << "Loading " << filename << std::endl;
	  string err = tinyobj::LoadObj(shapes, materials, filename, basepath);
	  if (!err.empty()) {
	    std::cerr << err << std::endl;
	    return false;
	  }
	  mtlpath = basepath;
	  return true;
	}

	void Draw()
	{
		
		glEnableClientState(GL_VERTEX_ARRAY);	
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		tinyobj::mesh_t mesh;
		for(int i=0;i<shapes.size();++i)
		{
			mesh = shapes[i].mesh;

			glVertexPointer(3,GL_FLOAT, 0 , &(mesh.positions[0]));
			glNormalPointer(GL_FLOAT, 0, &(mesh.normals[0]));	
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, &(mesh.indices[0]));		
		//
		}		
			glDisableClientState(GL_VERTEX_ARRAY);	
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	void DrawColor()
	{
		glEnableClientState(GL_VERTEX_ARRAY);	
		glEnableClientState(GL_NORMAL_ARRAY);
		tinyobj::mesh_t mesh;
		int mat;
		int oldMat = -1;
		for(int i=0;i<shapes.size();++i)
		{
			mesh = shapes[i].mesh;
			mat = mesh.material_ids[0];
			if(mat !=oldMat)
			{
				//cout<<materials[mat].name<<endl;
				oldMat = mat;
				glMaterialfv(GL_FRONT, GL_AMBIENT, materials[mat].ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[mat].diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, materials[mat].specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, &materials[mat].shininess);
			}	
			glVertexPointer(3,GL_FLOAT, 0 , &(mesh.positions[0]));
			if(!mesh.normals.empty())glNormalPointer(GL_FLOAT, 0, &(mesh.normals[0]));	
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, &(mesh.indices[0]));		
		//
		}		
			glDisableClientState(GL_VERTEX_ARRAY);	
			glDisableClientState(GL_NORMAL_ARRAY);
	}

	void DrawTexture()
	{
		
		glEnableClientState(GL_VERTEX_ARRAY);	
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		tinyobj::mesh_t mesh;
		int mat;
		int oldMat = -1;

		for(int i=0;i<shapes.size();++i)
		{
			mesh = shapes[i].mesh;
			mat = mesh.material_ids[0];
			if(mat !=oldMat)
			{
				oldMat = mat;
				glMaterialfv(GL_FRONT, GL_AMBIENT, materials[mat].ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[mat].diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, materials[mat].specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, &materials[mat].shininess);
			}	
		//	glBindTexture(GL_TEXTURE_2D, texmaps[materials[mat].name]);	
			glVertexPointer(3,GL_FLOAT, 0 , &(mesh.positions[0]));
			if(!mesh.normals.empty())glNormalPointer(GL_FLOAT, 0, &(mesh.normals[0]));
			
			if(!mesh.texcoords.empty())glTexCoordPointer(3,GL_FLOAT, 0, &mesh.texcoords[0]);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, &(mesh.indices[0]));		
		//
		}		
			glDisableClientState(GL_VERTEX_ARRAY);	
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	}
};

Model_OBJ * object;
//Model_OBJ * object2;
void display()
{
	glClearColor(0.7215,0.8627,0.9490,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLoadIdentity();
		glTranslatef(0, -1, -10);
	//	glRotatef(-90,0,1,0);
	//glColor3f(1,1,1);
	//glRotatef(90,0.0f,1.0,0.0f);
	glRotatef(gol,0.0f,1.0f,0.0f);
	object->DrawTexture();
	glTranslatef(2,2,2);
	
	//object2->Draw();
	glutSwapBuffers(); //swap the buffers
	glFlush();
}

void initGL()
{
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);			// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

	glClearColor(0.5,0.5,0.5,1.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45,640.0/480.0,1.0,1000.0);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        float col[]={1.0,1.0,1.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
	glLightfv(GL_LIGHT0, GL_SPECULAR, col);
	glLightfv(GL_LIGHT0, GL_AMBIENT, col);
	///draw
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        float pos[]={-1.0,1.0,-2.0,1.0};
        glLightfv(GL_LIGHT0,GL_POSITION,pos);
	
}

int main(int argc, char **argv)
{
	
	//win.z_near = 1.0f;
	object = new Model_OBJ;
	//object2 = new Model_OBJ;
	glutInit(&argc,argv);
	object->Load("../Avent.obj","../");
	//object2->Load("../track5.obj","../");
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(1024,768);
	glutInitWindowPosition(20,20);
	glutCreateWindow("ObjLoader");
	initGL();
	object->LoadTextures();
	//glutKeyboardFunc( keyboard );
	//glutKeyboardUpFunc(keyUp);
	//		glutFullScreen();
	//glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	//glutIdleFunc(display);
	glutMainLoop();

	return 0;
		
}

#include "gasket.h"
#include <GL/gl.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <iostream>

#define _GK_GL3CORE_H
#include "CallBackglBegin.hpp"


Gasket::Gasket()
{
    
}

namespace
{
	struct FColor final
	{
		float r;
		float g;
		float b;
	};

	void drawCurrentMesh(const Mesh& mesh, int lineWidth)
	{
		glLineWidth(lineWidth);
		glColor3f(1.0f, 1.0f, 1.0f);
		mtl::gl::begin(GL_TRIANGLES, [&mesh](void){
			auto vertices  = mesh.getVertices();
			auto triangles = mesh.getTriangles();
			for(auto tr : triangles)
			{
				for(auto index=tr.beginVertice();index!=tr.endVertice();++index)
				{
					glVertex3dv(vertices.at(*index));
				}
			}
		});
	}
	
	void drawEdges(const Mesh& mesh, const Curve_c& curve, int lineWidth, const FColor& c)
	{
		glLineWidth(lineWidth);
		glColor3f(c.r, c.g, c.b);
		mtl::gl::begin(GL_LINES, [&curve, &mesh](){
			for(auto edge : curve)
			{
				Vertex tmp = mesh.getVertices().at(edge.a);
				glVertex3dv(tmp);
				tmp = mesh.getVertices().at(edge.b);
				glVertex3dv(tmp);
			}
		});
	}
	
	/*
	void drawCircle(const Vertex& center, double radius, int lineWidth)
	{
		int num_segments = 100;
		glLineWidth(lineWidth);
		mtl::gl::begin(GL_LINE_LOOP, [num_segment, &radius](void){
			for(int ii = 0; ii < num_segments;++ii)
			{
				double theta = 2.0f * 3.1415926f * double(ii) / double(num_segments);
				double x = radius * cosf(theta);
				double y = radius * sinf(theta);
				glVertex3d(x + center.x(), y + center.y(), center.z());
			}
		});
	}
	*/
	
	void drawVertices(const Mesh& mesh, int poinstSize, float r, float g, float b, uint32_t beg, uint32_t end)
	{
		glPointSize(poinstSize);
		glColor3f(r, g, b);
		mtl::gl::begin(GL_POINTS, [beg, end, &mesh](void){
			for(uint32_t i=beg;i<end;++i)
			{
				Vertex tmp = mesh.getVertices().at(i);
				glVertex3dv(tmp);
			}
		});
	}

}
void Gasket::draw()
{
	if (this->config.type == MESH || this->config.type == TRIANGULATION)
	{
		if (this->config.type == TRIANGULATION)
		{
			drawVertices(this->mesh, 8, 1.0f, 1.0f, 0.0f, 0, this->mesh.getVertices().size());
		}
		drawCurrentMesh(this->mesh, 2);
	}
	else if (this->config.type == CURVE)
	{
		if (this->config.points)
		{
			drawVertices(this->mesh, 8, 1.0f, 1.0f, 0.0f, 0, this->mesh.getIndexBeforeVoronoi());
		}
		if (this->config.centers)
		{
			drawVertices(this->mesh, 8, 0.0f, 0.0f, 1.0f, this->mesh.getIndexBeforeVoronoi(), this->mesh.getVertices().size());
		}
		if (this->config.curve)
		{
			FColor c = {1.0f, 0.0f, 0.0f};
			drawEdges(this->mesh, this->mesh.getCurve(), 3, c);
		}
		if (this->config.triangles)
		{
			drawCurrentMesh(this->mesh, 2);
		}
		if (this->config.circles)
		{
			
		}
		
	}
	else if (this->config.type == CONSTRAINTS)
	{
		drawVertices(this->mesh, 8, 0.0f, 0.5f, 1.0f, 0, this->mesh.getVertices().size());
		FColor c = {1.0, 0.0, 0.0};
		drawEdges(this->mesh, this->mesh.getConstraints(), 4, c);
		drawCurrentMesh(this->mesh, 2);
	}
}

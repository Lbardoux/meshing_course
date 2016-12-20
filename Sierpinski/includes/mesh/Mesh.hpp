/**
 * @file Mesh.hpp
 * @brief This defines a Mesh class which could parse to .off or .pts
 * files to load 3D meshes, or 2D triangulation.
 * @author MTLCRBN
 * @version 2.0
 */
#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <vector>
#include <string>
#include <list>

#include "file_io.hpp"
#include "predicats.hpp"

// Topology
#include "common.hpp"
#include "Vertex3D.hpp"
#include "TopoTriangle.hpp"

// Iterators
#include "TriangleIterator.hpp"
#include "VertexIterator.hpp"
#include "TriangleCirculator.hpp"
#include "VertexCirculator.hpp"


/**
 * @class Mesh
 * @brief Defines a multifunction mesh.
 */
class Mesh final
{
	public:
		// ## Loading/emptying data ##############################################
		/**
		 * @brief Load a 3D mesh from a well formated OFF file named \p fname.
		 * @param[in] fname The file name of your off.
		 * @pre \p fname must refers to a valid file.
		 */
		void loadMeshFromOff(const std::string& fname);
		/**
		 * @brief Empty every data inside this Mesh.
		 */
		void empty(void);
		/**
		 * @brief Dump the content of this Mesh into an OFF file named \p fname.
		 * @param[in] fname The name of the OFF file you wanna write.
		 */
		void dumpToOff(const std::string& fname) const;
		/**
		 * @brief Load the 2D triangulation fro ma PTS file named \p fname.
		 * @param[in] fname The name of the PTS file you wanna load.
		 */
		void load2DTriangulationFromPts(const std::string& fname);
		/**
		 * @brief Load a Curve from a pts file named \p fname for crust algorithm.
		 * @param[in] fname The name of the PTS file you wanna load.
		 */
		void load2DCurve(const std::string& fname);
		
		
		// #######################################################################
		
		/**
		 * @brief Flip \p f1 with \p f2.
		 * @param[in] f1 The index of the first  triangle to be flip.
		 * @param[in] f2 The index of the second triangle to be flip.
		 */
		void flip(IndexFace_t f1, IndexFace_t f2);
		
		// ## Iterators ##########################################################
		typedef _TriangleIterator   triangle_iterator;   //!< To offer a simple name for this iterator over triangles.
		typedef _VertexIterator     vertex_iterator;     //!< To offer a simple name for this iterator over vertices.
		typedef _TriangleCirculator triangle_circulator; //!< To offer a simple name for this circulator over triangles.
		typedef _VertexCirculator   vertex_circulator;   //!< To offer a simple name for this circulator over vertices.
		
		/**
		 * @brief Create an iterator through triangle ( \b T for triangle !).
		 * @return A iterator to the beginning of the triangles.
		 */
		triangle_iterator beginT(void);
		/**
		 * @brief Create an iterator through triangle ( \b T for triangle !).
		 * @return A iterator to the ending of the triangles.
		 * @warning Don't dereference this iterator !
		 */
		triangle_iterator endT(void);
		/**
		 * @brief Create an iterator through vertex ( \b V for vertex !).
		 * @return A iterator to the beginning of the vertices.
		 */
		vertex_iterator beginV(void);
		/**
		 * @brief Create an iterator through vertices ( \b V for vertex !).
		 * @return A iterator to the ending of the vertices.
		 * @warning Don't dereference this iterator !
		 */
		vertex_iterator endV(void);
		/**
		 * @brief Create a circulator through vertex ( \b V for vertex !).
		 * @param[in] center The index of the vertex you wanna use as an anchor
		 * @return A iterator to the beginning of the vertices.
		 */
		vertex_circulator beginRV(IndexVertex_t center);
		/**
		 * @brief Create a circulator through triangles ( \b T for triangle !).
		 * @param[in] center The index of the vertex you wanna use as an anchor
		 * @return A iterator to the beginning of the vertices.
		 */
		triangle_circulator beginRT(IndexVertex_t center);
		// #######################################################################
		
		// ## Getters ############################################################
		      VertexContainer& getVertices(void);
		const VertexContainer& getVertices(void) const;
		
		      TriangleContainer& getTriangles(void);
		const TriangleContainer& getTriangles(void) const;
		
		      Border_c& getBorders(void);
		const Border_c& getBorders(void) const;
		
		      Curve_c& getCurve(void);
		const Curve_c& getCurve(void) const;
		
		      Curve_c& getConstraints(void);
		const Curve_c& getConstraints(void) const;
		
		inline int32_t getIndexBeforeVoronoi(void) const{return this->indexBeforeVoronoi;}
		// #######################################################################
		void incrementalDelaunay(const std::vector<IndexFace_t>& newTriangles);

		/**
		 * @brief Apply crust algorithm, and return a bunch of edges.
		 * @pre The mesh must contains some triangles at this point, of Delaunay.
		 */
		void Crust(void);
		
		/**
		 * @brief Load a .ctri file \b fname with this format :
		 * number_of_vertex
		 * 2D vertices .... x number_of_vertex
		 * a b ... x number_of_vertex
		 * @param[in] fname The name of the file
		 * @pre \b fname must de a valid file.
		 * @post The constraint delaunay is up to be shown.
		 */
		void loadConstraints(const std::string& fname);
		
		
		
	private:
		VertexContainer   vertices;           //!< Every vertices  of this mesh.
		TriangleContainer triangles;          //!< Every triangles of this mesh.
		Border_c          borders;            //!< Every indexes of the vertices on the edges of the triangulation.
		Curve_c           curve;              //!< The edges for the curve.
		Curve_c           constraints;        //!< The edges for the curve.
		int32_t           indexBeforeVoronoi; //!< The index where the voronoi centers are store.
		
		/**
		 * @brief Read \b nb vertex from \b file, and insert them into an incremental delaunay triangulation.
		 * @param file The previously opened file to read.
		 * @param nb   The number of vertex       to read.
		 */
		void readVerticesFromPts(InputFile& file, uint32_t nb);
		/**
		 * @brief Insert \b v with \b index as vertex' index into the triangulation.
		 * @param v     The vertex to insert.
		 * @param index It index.
		 */
		void insertVertexIntoTriangulation(Vertex& v, IndexVertex_t index);
		/**
		 * @brief Create the first triangle and initialize the border.
		 * It's the start point of the 2D triangulation.
		 */
		void createInitialTriangle(void);
		/**
		 * @brief Check if \b v belongs to any triangle of the triangulation.
		 * @param[in] v The vertex to check with. 
		 * @return A value between [0, triangles.size()[ if \b v belongs to a triangle, -1 otherwise.
		 */
		IndexFace_t isInOneTriangle(const Vertex& v);
		/**
		 * @brief Prepare a triangle \b t to be use with predicats
		 * @param[in] t The triangle to convert into a predicat' triangle.
		 * @return This Ptriangle for any predicat.
		 */
		Ptriangle3D buildPtriangle3D(TopoTriangle& t);
		/**
		 * @brief Find a triangle which contains \b a and \b b.
		 * @param[in] a One     vertex index to check with.
		 * @param[in] b Another vertex index to check with.
		 * @return A valid IndexFace_t is something was find, -1 otherwise.
		 */
		IndexFace_t findThisFace(IndexVertex_t a, IndexVertex_t b) const;
		/**
		 * @brief Case when you need to insert a vertex into an existing triangle.
		 * @param[in,out] v                The vertex to insert.  
		 * @param[in]     indexCurrentFace The face where we'll insert \b v.
		 * @param[in]     v_index          v's index.
		 */
		void insertPointIntoTriangle(Vertex& v, IndexFace_t indexCurrentFace, IndexVertex_t v_index);
		/**
		 * @brief Manage neighborhood for an insertion inside a triangle.
		 * @param[in]     news      The newly created triangles indexes.
		 * @param[in,out] concerned The indexes of the direct neighbors.
		 */
		void manageNeighborInside(const std::vector<IndexFace_t> &news, std::vector<IndexFace_t>& concerned);
		/**
		 * @brief Check if the triangle of index \b tr_id is localy de Delaunay
		 * @param[in] tr_id The index of the triangle you wanna check.
		 * @return -1 if \b tr_id is localy de Delaunay, the neighbor index which conflicts otherwise.
		 */
		IndexFace_t localDelaunay(IndexFace_t tr_id);
		/**
		 * @brief Update the border container (remove or add)
		 * @param[in] id     The inserted vertex' index.
		 * @param[in] usages The vertices which was use for the insertion (1 --> used 1 time)
		 * @param[in] first  The first used index.
		 */
		void updateBorder(IndexVertex_t id, const std::vector<uint32_t>& usages, IndexVertex_t first);
		/**
		 * @brief Deals with neighborhood for an outside of a triangle insertion.
		 * @param[in] newTriangles The newly created triangles indexes.
		 * @param[in] c            The index of the inserted vertex.
		 */
		void updateNeighborsOutside(std::vector<IndexFace_t>& newTriangles, IndexVertex_t c);
		/**
		 * @brief Case when you need to insert a point outside any triangle.
		 * @param[in,out] ins   The vertex to insert.
		 * @param[in]     index It index.
		 */
		void insertPointOutside(Vertex& ins, IndexVertex_t index);
		/**
		 * @brief Read the header of a .pts || .tri || .ctri, and then read the 2D vertice.
		 * @param[in,out] file The prealably opened file, with a valid format.
		 */
		void loadVertices(InputFile& file);
		/**
		 * @brief Use the constraints to refine a de Delaunay triangulation with ruppert's algorithm.
		 * @param[in] threshold The minimal value of any angle from a triangle.
		 */
		void refineDelaunay(double threshold);
		/**
		 * @brief Compute a vertex which is the center of \b edge.
		 * @param[in] edge The edge you wanna split.
		 * @return A vertex which represents this center.
		 */
		Vertex centerOfEdge(const TopoTriangle::Edge& edge);
		/**
		 * @brief Puts every bad triangle (with an angle lower than \b threshold) into a container.
		 * @param[in] threshold The minimal value of any angle from a triangle.
		 * @return A container which contains the index of the poor quality triangles.
		 */
		std::list<IndexFace_t> collectPoorQualityTriangles(double threshold);
		/**
		 * @brief Parse the constraint segment list to get the encroached segments
		 * @param[out] notEncroached The container for not encroached segments.
		 * that means segments that's not included into our current delaunay triangulation. 
		 * @return The list of the encroached segments.
		 */
		Curve_c collectEncroacheds(Curve_c& notEncroached);
		/**
		 * @brief Check if \b v encroaches any segment of \b segments, and store the result into \b edge.
		 * It will remove the conflicted segment from the container.
		 * @param[in]     v        The vertex to test with.
		 * @param[in,out] segments The segment container of the "valid" constraints.
		 * @param[out]    edge     The storage place for the result if needed.
		 * @return true if a segment is encroach (filling up \b edge), false otherwise.
		 */
		bool encroachSegment(const Vertex& v, Curve_c& segments, TopoTriangle::Edge& edge);

};

#endif

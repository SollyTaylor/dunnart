/*
 * $Revision: 1.4 $
 * 
 * last checkin:
 *   $Author: gronemann $ 
 *   $Date: 2009-12-15 03:20:52 +1100 (Tue, 15 Dec 2009) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of classes GmlObject and GmlParser.
 * 
 * \author Carsten Gutwenger
 * 
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2007
 * 
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
 * 
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * \par
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 * 
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/


#ifdef _MSC_VER
#pragma once
#endif

#ifndef OGDF_GML_PARSER_H
#define OGDF_GML_PARSER_H


#include <ogdf/basic/Hashing.h>
#include <ogdf/basic/String.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/cluster/ClusterGraph.h>
#include <ogdf/cluster/ClusterGraphAttributes.h>


namespace ogdf {


typedef HashElement<String,int> *GmlKey;
enum GmlObjectType { gmlIntValue, gmlDoubleValue, gmlStringValue, gmlListBegin,
	gmlListEnd, gmlKey, gmlEOF, gmlError };


//---------------------------------------------------------
// GmlObject
// represents node in GML parse tree
//---------------------------------------------------------
struct OGDF_EXPORT GmlObject {
	GmlObject *m_pBrother; // brother of node in tree
	GmlKey m_key; // tag of node
	GmlObjectType m_valueType; // type of node

	// the entry in the union is selected according to m_valueType:
	//   gmlIntValue -> m_intValue
	//   gmlDoubleValue -> m_doubleValue
	//   gmlStringValue -> m_stringValue
	//   gmlListBegin -> m_pFirstSon (in case of a list, m_pFirstSon is pointer
	//     to first son and the sons are chained by m_pBrother)
	union {
		int m_intValue;
		double m_doubleValue;
		const char *m_stringValue;
		GmlObject *m_pFirstSon;
	};

	// construction
	GmlObject(GmlKey key, int intValue) : m_pBrother(0), m_key(key),
		m_valueType(gmlIntValue), m_intValue(intValue)  { }

	GmlObject(GmlKey key, double doubleValue) : m_pBrother(0), m_key(key),
		m_valueType(gmlDoubleValue), m_doubleValue(doubleValue)  { }

	GmlObject(GmlKey key, const char *stringValue) : m_pBrother(0), m_key(key),
		m_valueType(gmlStringValue), m_stringValue(stringValue)  { }

	GmlObject(GmlKey key) : m_pBrother(0), m_key(key),
		m_valueType(gmlListBegin), m_pFirstSon(0)  { }

	OGDF_NEW_DELETE
};


//---------------------------------------------------------
// GmlParser
// reads GML file and constructs GML parse tree
//---------------------------------------------------------
class OGDF_EXPORT GmlParser {
 	int m_lineNumber;
	Hashing<String,int> m_hashTable; // hash table for tags
	int m_num;

	istream *m_is;
	bool m_error;
	String m_errorString;

	char *m_rLineBuffer, *m_lineBuffer, *m_pCurrent, *m_pStore, m_cStore;

	int m_intSymbol;
	double m_doubleSymbol;
	const char *m_stringSymbol;
	GmlKey m_keySymbol;
	String m_longString;

	GmlObject *m_objectTree; // root node of GML parse tree

	bool m_doCheck;
	Array<node> m_mapToNode;
	GmlObject  *m_graphObject;

public:
	// predefined id constants for all used keys
	enum PredefinedKey { idPredefKey = 0, labelPredefKey, CreatorPredefKey,
		namePredefKey, graphPredefKey, versionPredefKey, directedPredefKey,
		nodePredefKey, edgePredefKey, graphicsPredefKey, xPredefKey,
		yPredefKey, wPredefKey, hPredefKey, 
		imagePredefKey, imageurlPredefKey,
		typePredefKey, widthPredefKey,
		sourcePredefKey, targetPredefKey, arrowPredefKey, LinePredefKey,
		pointPredefKey, generalizationPredefKey, subGraphPredefKey, fillPredefKey, clusterPredefKey,
		rootClusterPredefKey, vertexPredefKey, colorPredefKey, 
		heightPredefKey, stipplePredefKey, patternPredefKey, 
		linePredefKey, lineWidthPredefKey, templatePredefKey,
		edgeWeightPredefKey, ignorePredefKey, NEXTPREDEFKEY };

	// construction: creates object tree
	// sets m_error flag if an error occured
	GmlParser(const char *fileName, bool doCheck = false);
	GmlParser(istream &is, bool doCheck = false);

	// destruction: destroys object tree
	~GmlParser();

	// returns id of object
	int id(GmlObject *object) const { return object->m_key->info(); }

	// true <=> an error in GML files has been detected
	bool error() const { return m_error; }
	// returns error message
	const String &errorString() const { return m_errorString; }
	int getLineNumber() const { return m_lineNumber; }

	// creates graph from GML parse tree
	bool read(Graph &G);
	// creates attributed graph from GML parse tree
	bool read(Graph &G, GraphAttributes &AG);
	//creates clustergraph from GML parse tree
	//bool read(Graph &G, ClusterGraph & CG);
	//read only cluster part of object tree and create cluster graph structure
	bool readCluster(Graph &G, ClusterGraph& CG);
	//the same with attributes 
	bool readAttributedCluster(Graph &G, ClusterGraph& CG,
									   ClusterGraphAttributes& ACG);

protected:

	//read all cluster tree information
	bool clusterRead(GmlObject* rootCluster, 
					 ClusterGraph& CG);
	//with attributes
	bool attributedClusterRead(GmlObject* rootCluster, 
							ClusterGraph& CG, 
							ClusterGraphAttributes& ACG);
	//recursively read cluster subtree information
	bool recursiveClusterRead(GmlObject* clusterObject,
	 					 ClusterGraph& CG,
						 cluster c);
	bool recursiveAttributedClusterRead(GmlObject* clusterObject,
								ClusterGraph& CG,
								ClusterGraphAttributes& ACG,
								cluster c);
	bool readClusterAttributes(GmlObject* cGraphics,
									  cluster c,
									  ClusterGraphAttributes& ACG);

private:
	void createObjectTree(istream &is, bool doCheck);
	void initPredefinedKeys();
	void setError(const char *errorString);

	GmlObject *parseList(GmlObjectType closingKey, GmlObjectType errorKey);
	GmlObjectType getNextSymbol();
	bool getLine();

	GmlKey hashString(const String &str);

	GmlObject *getNodeIdRange(int &minId,int &maxId);
	void readLineAttribute(GmlObject *object, DPolyline &dpl);

	void destroyObjectList(GmlObject *object);

	void indent(ostream &os, int d);
	void output(ostream &os, GmlObject *object, int d);

};


} // end namespace ogdf

#endif

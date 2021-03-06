/*
 * $Revision: 1.0 $
 *
 * last checkin:
 *   $Author: bartel $
 *   $Date: 2008-01-26 15:44:58 +0100 (Sa, 26 Jan 2008) $
 ***************************************************************/

/** \file
 * \brief useable example of the Modular Multilevel Mixer
 *
 * \author Gereon Bartel
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

#include <ogdf/energybased/multilevelmixer/MMMExampleFastLayout.h>
#include <ogdf/basic/PreprocessorLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>
#include <ogdf/energybased/multilevelmixer/ModularMultilevelMixerLayout.h>
#include <ogdf/energybased/multilevelmixer/ScalingLayout.h>
#include <ogdf/energybased/FastMultipoleEmbedder.h>

#include <ogdf/energybased/multilevelmixer/SolarMerger.h>
#include <ogdf/energybased/multilevelmixer/SolarPlacer.h>

namespace ogdf {

MMMExampleFastLayout::MMMExampleFastLayout()
{
}


void MMMExampleFastLayout::call(GraphAttributes &GA)
{
	MultilevelGraph MLG(GA);
	call(MLG);
	MLG.exportAttributes(GA);
}


void MMMExampleFastLayout::call(MultilevelGraph &MLG)
{
	// Fast Multipole Embedder
	ogdf::FastMultipoleEmbedder * FME = new ogdf::FastMultipoleEmbedder();
	FME->setNumIterations(1000);
	FME->setRandomize(false);

	// Solar Merger
	ogdf::SolarMerger * SM = new ogdf::SolarMerger(false, false);

	// Solar Placer
	ogdf::SolarPlacer * SP = new ogdf::SolarPlacer();

	// No Scaling
	ogdf::ScalingLayout * SL = new ogdf::ScalingLayout();
	SL->setExtraScalingSteps(0);
	SL->setScaling(2.0, 2.0);
	SL->setScalingType(ogdf::ScalingLayout::st_relativeToDrawing);
	SL->setSecondaryLayout(FME);
	SL->setLayoutRepeats(1);

	ogdf::ModularMultilevelMixer MMM;
	MMM.setLayoutRepeats(1);
//	MMM.setAllEdgeLenghts(5.0);
//	MMM.setAllNodeSizes(1.0);
	MMM.setLevelLayoutModule(SL);
	MMM.setInitialPlacer(SP);
	MMM.setMultilevelBuilder(SM);

	// set Postprocessing Options
	MMM.setPostLayoutModule(0);
	MMM.setPostTimeFactor(0);
	MMM.setPostIterations(0);
	MMM.setPostProcessingAfterEveryStep(false);

	ogdf::TileToRowsCCPacker TTRCCP;
	ogdf::ComponentSplitterLayout CS(TTRCCP);
	CS.setLayoutModule(MMM);
	ogdf::PreprocessorLayout PPL;
	PPL.setLayoutModule(CS);
	PPL.setRandomizePositions(true);

	PPL.call(MLG);
}

} // namespace ogdf


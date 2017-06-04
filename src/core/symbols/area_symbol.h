/*
 *    Copyright 2012, 2013 Thomas Schöps
 *    Copyright 2012-2017 Kai Pastor
 *
 *    This file is part of OpenOrienteering.
 *
 *    OpenOrienteering is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    OpenOrienteering is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with OpenOrienteering.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef OPENORIENTEERING_AREA_SYMBOL_H
#define OPENORIENTEERING_AREA_SYMBOL_H

#include "symbol.h"

class PointObject;


/**
 * Symbol for PathObjects where the enclosed area is filled with a solid color
 * and / or with one or more patterns.
 */
class AreaSymbol : public Symbol
{
friend class AreaSymbolSettings;
friend class PointSymbolEditorWidget;
friend class OCAD8FileImport;
public:
	/** Describes a fill pattern. */
	struct FillPattern
	{
		/** Types of fill patterns. */
		enum Type
		{
			/** Parallel lines pattern */
			LinePattern = 1,
			/** Point grid pattern */
			PointPattern = 2
		};
		
		/** Type of the pattern */
		Type type;
		/** Rotation angle in radians */
		float angle;
		/** True if the pattern is rotatable per-object. */
		bool rotatable;
		/** Distance between parallel lines, as usual in 0.001mm */
		int line_spacing;
		/** Offset of the first line from the origin */
		int line_offset;
		
		// For type == LinePattern only:
		
		/** Line color */
		const MapColor* line_color;
		/** Line width */
		int line_width;
		
		// For type == PointPattern only:
		
		/** Offset of first point along parallel lines */
		int offset_along_line;
		/** Point distance along parallel lines */
		int point_distance;
		/** Contained point symbol */
		PointSymbol* point;
		
		/** Display name (transient) */
		QString name;
		
		
		/** Creates a default fill pattern */
		FillPattern();
		/** Loads the pattern in the old "native" format */
		bool load(QIODevice* file, int version, Map* map);
		/** Saves the pattern in xml format */
		void save(QXmlStreamWriter& file, const Map& map) const;
		/** Loads the pattern in xml format */
		void load(QXmlStreamReader& xml, const Map& map, SymbolDictionary& symbol_dict);
		/**
		 * Checks if the pattern settings are equal to the other.
		 * TODO: should the transient name really be compared?!
		 */
		bool equals(const FillPattern& other, Qt::CaseSensitivity case_sensitivity) const;
		
		/**
		 * Creates renderables for this pattern in the area given by extent.
		 * @param extent Rectangular area to create renderables for.
		 * @param delta_rotation Rotation offest which is added to the pattern angle.
		 * @param pattern_origin Origin point for line / point placement.
		 * @param output Created renderables will be inserted here.
		 */
		void createRenderables(
			QRectF extent,
			float delta_rotation,
			const MapCoord& pattern_origin,
			ObjectRenderables& output
		) const;
		
		/** Creates one line of renderables, called by createRenderables(). */
		void createLine(
			MapCoordF first, MapCoordF second,
			float delta_offset,
			LineSymbol* line,
			float rotation,
			ObjectRenderables& output
		) const;
		/** Spatially scales the pattern settings by the given factor. */
		void scale(double factor);
	};
	
	AreaSymbol();
	virtual ~AreaSymbol();
	Symbol* duplicate(const MapColorMap* color_map = NULL) const override;
	
	void createRenderables(
	        const Object *object,
	        const VirtualCoordVector &coords,
	        ObjectRenderables &output,
	        Symbol::RenderableOptions options) const override;
	
	void createRenderables(
	        const PathObject* object,
	        const PathPartVector& path_parts,
	        ObjectRenderables &output,
	        Symbol::RenderableOptions options) const override;
	
	void createRenderablesNormal(
	        const PathObject* object,
	        const PathPartVector& path_parts,
	        ObjectRenderables& output) const;
	
	/**
	 * Creates area hatching renderables for a path object.
	 */
	void createHatchingRenderables(
	        const PathObject *object,
	        const PathPartVector& path_parts,
	        ObjectRenderables &output,
	        const MapColor* color) const;
	
	
	void colorDeleted(const MapColor* color) override;
	bool containsColor(const MapColor* color) const override;
	const MapColor* guessDominantColor() const override;
	void scale(double factor) override;
	
	// Getters / Setters
	inline const MapColor* getColor() const {return color;}
	inline void setColor(const MapColor* color) {this->color = color;}
	inline int getMinimumArea() const {return minimum_area; }
	inline int getNumFillPatterns() const {return (int)patterns.size();}
	inline void setNumFillPatterns(int count) {patterns.resize(count);}
	inline FillPattern& getFillPattern(int i) {return patterns[i];}
	inline const FillPattern& getFillPattern(int i) const {return patterns[i];}
	bool hasRotatableFillPattern() const;
	SymbolPropertiesWidget* createPropertiesWidget(SymbolSettingDialog* dialog) override;
	
protected:
#ifndef NO_NATIVE_FILE_FORMAT
	bool loadImpl(QIODevice* file, int version, Map* map) override;
#endif
	void saveImpl(QXmlStreamWriter& xml, const Map& map) const override;
	bool loadImpl(QXmlStreamReader& xml, const Map& map, SymbolDictionary& symbol_dict) override;
	bool equalsImpl(const Symbol* other, Qt::CaseSensitivity case_sensitivity) const override;
	
	const MapColor* color;
	int minimum_area;	// in mm^2 // FIXME: unit (factor) wrong
	std::vector<FillPattern> patterns;
};

#endif

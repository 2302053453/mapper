/*
 *    Copyright 2012, 2013 Thomas Schöps
 *    Copyright 2017 Kai Pastor
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


#ifndef OPENORIENTEERING_REPLACE_SYMBOL_SET_DIALOG_H
#define OPENORIENTEERING_REPLACE_SYMBOL_SET_DIALOG_H

#include <memory>

#include <QDialog>
#include <QHash>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QTableWidget;
QT_END_NAMESPACE

class Map;
class Symbol;
class SymbolDropDownDelegate;


/**
 * Dialog and tool for replacing the map's symbol set with another.
 * 
 * Lets the user choose options and possibly even choose the replacement
 * for every single symbol.
 */
class ReplaceSymbolSetDialog : public QDialog
{
Q_OBJECT
public:
	using SymbolMapping      = QHash<const Symbol*, Symbol*>;
	using ConstSymbolMapping = QHash<const Symbol*, const Symbol*>;
	
	/**
	 * Lets the user select a file to load the symbols from and shows the dialog.
	 * 
	 * Returns true if the replacement has been finished, false if aborted.
	 */
	static bool showDialog(QWidget* parent, Map* map);
	
	/**
	 * Shows the dialog for a given base map, imported map, and cross reference file.
	 * 
	 * The replacement takes place in the imported map.
	 * Returns true if the replacement has been finished, false if aborted.
	 */
	static bool showDialogForCRT(QWidget* parent, const Map* base_map, Map* imported_map, QIODevice& crt_file);
	
private slots:
	void matchByNumberClicked(bool checked);
	void showHelp();
	void apply();
	
private:
	enum Mode
	{
		ModeStandard,
		ModeCRT
	};
	
	ReplaceSymbolSetDialog(QWidget* parent, Map* map, const Map* symbol_map, Mode mode = ModeStandard);
	virtual ~ReplaceSymbolSetDialog();
	
	void calculateNumberMatchMapping();
	const Symbol* findNumberMatch(const Symbol* original, bool ignore_trailing_zeros);
	void updateMappingTable();
	void updateMappingFromTable();
	
	Map* map;
	const Map* symbol_map;
	ConstSymbolMapping mapping;
	Mode mode;
	
	QCheckBox* import_all_check;
	QCheckBox* delete_unused_symbols_check;
	QCheckBox* delete_unused_colors_check;
	QCheckBox* preserve_symbol_states_check;
	QCheckBox* match_by_number_check;
	QTableWidget* mapping_table;
	std::vector<std::unique_ptr<SymbolDropDownDelegate>> symbol_widget_delegates;
};

#endif

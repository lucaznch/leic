package xxl.core;

import java.io.Serial;
import java.io.Serializable;
import java.util.*;
import xxl.core.exception.UnrecognizedEntryException;



public class Spreadsheet implements Serializable {
    private final int _numRows, _numColumns;
    private Cell[][] _spreadsheet;
    private transient Parser _parser;
    private boolean _isSpreadsheetSaved;
    private CutBuffer _buffer;

    @Serial
    private static final long serialVersionUID = 202308312359L;

    

    public Spreadsheet(int numRows, int numColumns) {
        _numRows = numRows;
        _numColumns = numColumns;
        _spreadsheet = new Cell[numRows][numColumns];
        _parser = new Parser(this);

        int maxRangeSize = (numRows > numColumns) ? numRows : numColumns; 
        _buffer = new CutBuffer(maxRangeSize);
    }


    /*
    * gets the cell of the specified address
    *
    * @param row - row of the cell
    * @param column - column of the cell
    */
    public Cell getCell(int row, int column) {
        Cell c = _spreadsheet[row - 1][column - 1];

        if (c == null) {
            c = new Cell(row, column);
            _spreadsheet[row - 1][column - 1] = c; /* updating cell in matrix */
        }

        return c;
    }


    /** @return - true if saved, false if not */
    public boolean getSavedStatus() {
        return _isSpreadsheetSaved;
    }


    /**
    * sets the status if the spreadsheet is saved
    * everytime there is new information in the spreadsheet, we must set the status to false (not saved)
    */
    public void setSavedStatus(boolean status) {
        _isSpreadsheetSaved = status;
    }


    /*
    * insert specified content in specified range
    *
    * @param range - range of cell(s) to change
    * @param contentSpecification - specifiation in a string format of the content to put in the specified cell(s)
    */
    public void insertContent(Range range, String contentSpecification) throws UnrecognizedEntryException {
        int row = range.getAddress(1);
        int column = range.getAddress(2);
        Content newContent = _parser.parseContent(contentSpecification);
        
        if (range.isOneCell()) {
            insertContent(row, column, newContent);
        }
        else {
            if (range.isRowInterval()) {
                for (int i = 0; i < range.getNumberOfCellsInRange(); i++) {
                    insertContent(row, column + i, newContent);
                }
            }
            else {
                for (int i = 0; i < range.getNumberOfCellsInRange(); i++) {
                    insertContent(row + i, column, newContent);
                }
            }
        }
    }


    /*
    * insert content in specified address
    * @param row - row of the cell
    * @param column - column of the cell
    * @param content - content to insert in cell
    */
    public void insertContent(int row, int column, Content content) {
        Cell cell = getCell(row, column);
        cell.setContent(content);
        setSavedStatus(false);
    }


    /**
    * searches in spreadsheet for cells with specific integer value
    * 
    * @param value - integer value of target search
    * @return - array of strings, each with the specified format to be displayed
    */
    public String[] searchIntValue(int value) {
        List<Cell> cells = new ArrayList<>();
        
        for (int i = 0; i < _numRows; i++) {
            for (int j = 0; j < _numColumns; j++) {
                Cell c = _spreadsheet[i][j];
                
                if (c != null) {
                    try {
                        if (c.value().asInt() == value) {
                            cells.add(c);
                        }
                    }
                    catch (NumberFormatException e) {
                        /** do nothing */
                    }
                }
            }
        }
        return view(cells);
    }


    /**
    * searches in spreadsheet for cells with specific string value
    * 
    * @param value - string value of target search
    * @return - array of strings, each with the specified format to be displayed
    */
    public String[] searchStringValue(String value) {
        List<Cell> cells = new ArrayList<>();
        
        for (int i = 0; i < _numRows; i++) {
            for (int j = 0; j < _numColumns; j++) {
                Cell c = _spreadsheet[i][j];
                
                if (c != null) {
                    try {
                        if (c.value().asString().equals(value)) {
                            cells.add(c);
                        }
                    }
                    catch (NumberFormatException e) {
                        /** do nothing */
                    }
                }
            }
        }
        return view(cells);
    }


    /**
    * searches in spreadsheet for all cells with specific function that contains given string 
    * 
    * @param functionName - full name or part of name of target function search
    * @return - array of strings, each with the specified format to be displayed
    */
    public String[] searchFunction(String functionName) {
        List<Cell> cells = new ArrayList<>();
        
        for (int i = 0; i < _numRows; i++) {
            for (int j = 0; j < _numColumns; j++) {
                Cell c = _spreadsheet[i][j];

                if (c != null) {
                    try {
                        Function function = (Function)c.getContent();
                        if (function.getName().contains(functionName)) {
                            cells.add(c);
                        }
                    }
                    catch (ClassCastException e) {
                        /** do nothing */
                    }
                }
            }
        }
        return view(cells);
    }


    /*
    * specifies the string format to be displayed in each cell (display format: row;column|content)
    *
    * @param cells - list of cells to be displayed
    * @return - array of strings, each with the specified format to be displayed
    */
    public String[] view(List<Cell> cells) {
        String[] cellExpressions = new String[cells.size()];

        for (int i = 0; i < cells.size(); i++) {
            Cell c = cells.get(i);
            
            if (c.isEmpty()) 
                cellExpressions[i] = c.getRow() + ";" + c.getColumn() + "|";
            
            else {
                String expression = c.getContent().toString(); /** expression: 12, 'abc, =1;2, =ADD(1,1;2) for example */

                if (expression.charAt(0) == '=') /* if expression is a reference or function */ {

                    try {
                        cellExpressions[i] = c.getRow() + ";" + c.getColumn() + "|" + c.value() + c.getContent().toString();
                    }
                    catch (NullPointerException | NumberFormatException e) { 
                        /** 
                        * NullPointerException: if cell content is a reference to a cell not defined, c.value() throws Exception
                        * NumberFormatException: if cell content is a binary function, and one of args is a string, c.value() throws Exception
                        *                        if cell content is a range function, and one of the cells in the range is not an int, c.value() throws Exception
                        */
                        cellExpressions[i] = c.getRow() + ";" + c.getColumn() + "|#VALUE" + c.getContent().toString();
                    }
                }

                else { /** if expression is a literal */
                    cellExpressions[i] = c.getRow() + ";" + c.getColumn() + "|" + c.value();
                }
            }
        }
        return cellExpressions;
    }


    /*
    * specifies the string format to be displayed in each cell (display format: row;column|content)
    *
    * @param range - range of cell(s) to visualize
    * @return - array of strings, each with the specified format to be displayed
    */
    public String[] view(Range range) {
        List<Cell> cells = range.getCells(); /** gets all cells from the specific range */
        return view(cells);
    }


    /**
    * specifices the string format to be displayed in each cell of buffer
    * 
    * @return - array of strings, each with the specified format to be displayed
    */
    public String[] viewBuffer() {
        return view(_buffer.getCells());
    }


    /**
    * get the dimensions of spreadsheet: number of rows or columns
    *
    * @param whichDimension - 1 for number of rows; 2 for number of columns
    * @return - specific dimension
    */
    public int getDimensions(int whichDimension) {
        if (whichDimension == 1) {
            return _numRows;
        }
        else {
            return _numColumns;
        }
    }


    /*
    * creates a range of cell(s) from the specified description
    *
    * @param rangeDescription - specifiation in a string format of the range to create
    * @return - range createad from specified range description
    * @throws UnrecognizedEntryException if range to be created does not respect spreadsheet dimensions or is not a row/column interval
    */
    public Range createRange(String rangeDescription) throws UnrecognizedEntryException {
        int fromRow, fromColumn, toRow, toColumn;

        if (rangeDescription.indexOf(':') != -1) { /* if range has fromCell toCell, i.e, (1;2:1;6) */
            String[] rangeCoordinates = rangeDescription.split("[:;]");

            fromRow = Integer.parseInt(rangeCoordinates[0]);
            fromColumn = Integer.parseInt(rangeCoordinates[1]);
            toRow = Integer.parseInt(rangeCoordinates[2]);
            toColumn = Integer.parseInt(rangeCoordinates[3]);
        }
        else { /* if range is one Cell, i.e, (1;2) */
            String[] rangeCoordinates = rangeDescription.split(";");

            fromRow = toRow = Integer.parseInt(rangeCoordinates[0]);
            fromColumn = toColumn = Integer.parseInt(rangeCoordinates[1]);
        }

        /** if any of the rows or columns are go over the fixed size of rows and columns of the spreadsheet */
        if (fromRow > _numRows || toRow > _numRows || fromColumn > _numColumns || toColumn > _numColumns) {
            throw new UnrecognizedEntryException("Intervalo inválido: " + rangeDescription);
        }

        /** if the range does not form an interval of cells from the same line or column */
        if (fromRow != toRow && fromColumn != toColumn) {
            throw new UnrecognizedEntryException("Intervalo inválido: " + rangeDescription);
        }

        Range range = new Range(fromRow, fromColumn, toRow, toColumn, this);

        return range;
    }


    /** sets the parser of this spreadsheet */
    public void setParser(Parser p) {
        _parser = p;
    }


    /** @return - cut buffer of the spreadsheet */
    public CutBuffer getBuffer() {
        return _buffer;
    }
}
package xxl.core;

import java.io.Serial;
import java.io.Serializable;
import java.util.*;


public class Range implements Serializable {
    private Spreadsheet _spreadsheet;
    private int _fromRow, _fromColumn; /** coordinates of first cell */
    private int _toRow, _toColumn; /** coordinates of last cell */
    @Serial
    private static final long serialVersionUID =202308130102L;


    public Range(int fromRow, int fromColumn, int toRow, int toColumn) {
        _fromRow = fromRow;
        _fromColumn = fromColumn;
        _toRow = toRow;
        _toColumn = toColumn;
    }


    public Range(int fromRow, int fromColumn, int toRow, int toColumn, Spreadsheet spreadsheet) {
        this(fromRow, fromColumn, toRow, toColumn);
        _spreadsheet = spreadsheet;
    }


    /** sets a spreadsheet for this range to be associated with */
    public void setSpreadsheet(Spreadsheet spreadsheet) {
        _spreadsheet = spreadsheet;
    }


    /** @return - list of cells in this range */
    List<Cell> getCells() {
        List<Cell> cells;

        if (isOneCell()) {
            cells = new ArrayList<Cell>(1);
            cells.add(_spreadsheet.getCell(_fromRow, _fromColumn));
            return cells;
        }

        int row = _fromRow;
        int column = _fromColumn;
        int size = getNumberOfCellsInRange();
        cells = new ArrayList<Cell>(size);

        if (isRowInterval()) { /* range has cells from the same ro! */
            for (int i = 0; i < size; i++) {
                cells.add(_spreadsheet.getCell(row, column + i));
            }
        }
        else { /* range has cells from the same column */
            for (int i = 0; i < size; i++) {
                cells.add(_spreadsheet.getCell(row + i, column));
            }
        }

        return cells;
    }


    /**  
    * gets a specific address from this range
    * 
    * @param whichAddress - specification of address to return
    * 1 = _fromRow, 2 = _fromColumn, 3 = _toRow, 4 = _toColumn
    *
    * @return - address
    */
    public int getAddress(int whichAddress) {
        if (whichAddress == 1)
            return _fromRow;
        
        else if (whichAddress == 2)
            return _fromColumn;
        
        else if (whichAddress == 3)
            return _toRow;
        
        else if (whichAddress == 4)
            return _toColumn;
        
        else 
            return 0;
    }


    /** @return - if this range is from only one cell */
    public boolean isOneCell() {
        return _fromRow == _toRow && _fromColumn == _toColumn;
    }


    /** @return - if this range is from a row interval of cells */
    public boolean isRowInterval() {
        return _fromRow == _toRow;
    }


    /** @return - number of cells in this range */
    public int getNumberOfCellsInRange() {
        if (isRowInterval()) 
            return _toColumn - _fromColumn + 1;
        
        else 
            return _toRow - _fromRow + 1;
    }


    /**
    * this method is called when range is only 1 cell: insert buffer starting from this one cell
    *
    * @param buffer - list of cells of buffer
    * @param isRowInterval - true if buffer contains cells from a row interval 
    */
    public void insertBuffer(List<Cell> buffer, boolean isRowInterval) {
        int rows = _spreadsheet.getDimensions(1);
        int cols = _spreadsheet.getDimensions(2);        
        int toRow = _fromRow;
        int toColumn = _fromColumn;
        
        for (Cell c: buffer) {
            _spreadsheet.insertContent(toRow, toColumn, c.getContent());

            if (isRowInterval) {
                toColumn++;
            }
            else {
                toRow++;
            }

            if (toColumn > cols || toRow > rows) {
                break;
            }
        }
    }


    @Override
    public String toString() {
        String s = "" + _fromRow + ";" + _fromColumn;
        if (isOneCell()) {
            return s;
        } 
        else {
            return "(" + s + ":" + _toRow + ";" + _toColumn + ")";
        }
    }

}
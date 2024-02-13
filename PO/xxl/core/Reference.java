package xxl.core;

public class Reference extends Content {
    private int _row, _column;
    private Spreadsheet _spreadsheet;



    public Reference(int row, int column) {
        _row = row;
        _column = column;
    }


    public Reference(int row, int column, Spreadsheet spreadsheet) {
        this(row, column);
        _spreadsheet = spreadsheet;
    }
    

    /** @return - specification of Reference to a string */
    @Override
    public String toString() {
        return "=" + _row + ";" + _column;
    }

    
    /** @return - value of this reference */
    @Override
    Literal value() {
        return _spreadsheet.getCell(_row, _column).value();
    }
}
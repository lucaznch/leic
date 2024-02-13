package xxl.core;

import java.io.Serial;
import java.io.Serializable;

public class Cell implements Serializable {
    private final int _row, _column;
    private Content _content;

    @Serial
    private static final long serialVersionUID = 202308130102L;



    public Cell(int row, int column) {
        _row = row;
        _column = column;
    }


    public Cell(int row, int column, Content content) {
        this(row, column);
        _content = content;
    }

    
    /** sets the content of this cell */
    void setContent(Content content) {
        _content = content;
    }


    /** @return - content of this cell */
    Content getContent() {
        return _content;
    }


    /** @return - value of content, which is a literal */
    Literal value() {
        /*
        try {
            return _content.value();
        }
        catch (NullPointerException e) {
            return new LiteralString("#VALUE");
        }
        */
        return _content.value();
    }


    /** checks if cell is empty, i.e, if there is no content */
    public boolean isEmpty() {
        return _content == null;
    }


    /** @return - row of cell */
    public int getRow() {
        return _row;
    }


    /** @return - column of cell */
    public int getColumn() {
        return _column;
    }
}
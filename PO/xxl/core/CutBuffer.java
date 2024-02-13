package xxl.core;

import java.io.Serial;
import java.io.Serializable;
import java.util.*;
import xxl.core.exception.PasteException;


public class CutBuffer implements Serializable {
    private List<Cell> _buffer;
    private boolean _isRowInterval;
    @Serial
    private static final long serialVersionUID =202308130102L;


    /** @param size - max size a range of the spreadsheet can have */
    public CutBuffer(int size) {
        _buffer = new ArrayList<Cell>(size);
    }



    /** 
    * copies the contents of the given range and inserts it in buffer
    * 
    * @param range - specific range to copy contents
    */
    public void copy(Range range) {
        _buffer.clear();
        List<Cell> cells = range.getCells();

        /** adding copies of cells: modify the address so the buffer starts with 1;1 */
        if (range.isRowInterval()) {
            _isRowInterval = true;
            for (int i = 1; i <= cells.size(); i++) {
                _buffer.add(new Cell(1, i, cells.get(i - 1).getContent()));
            }
        }
        else {
            _isRowInterval = false;
            for (int i = 1; i <= cells.size(); i++) {
                _buffer.add(new Cell(i, 1, cells.get(i - 1).getContent()));
            }
        }
    }


    /**
    * deletes the contents of the given range
    *
    * @param range - specific range to delete contents
    */
    public void delete(Range range) {
        List<Cell> cellsFromRange = range.getCells();

        for (Cell c: cellsFromRange) {
            c.setContent(null);
        }
    }


    /**
    * cuts the contents of the given range 
    * 
    * @param range - specific range to cut contents
    */
    public void cut(Range range) {
        copy(range);
        delete(range);
    }


    /**
    * pastes the contents of buffer into given range
    * if buffer has no contents or if range size != buffer size => do nothing 
    * if range one cell => paste buffer starting from the cell respecting limits of spreadsheet 
    *
    * @param range - specific range to paste buffer contents
    */
    public void paste(Range range) throws PasteException {

        if (_buffer.size() == 0) { 
            return; /** if buffer empty then do nothing */
        }
        else if (range.isOneCell()) {
            range.insertBuffer(_buffer, _isRowInterval);
        }
        else if (range.getCells().size() == _buffer.size()) {
            List<Cell> cells = range.getCells();

            for (int i = 0; i < cells.size(); i++) {
                Cell c = cells.get(i);
                Content content = _buffer.get(i).getContent();
                c.setContent(content);
            }
        }
        else {
            return;
        }
    }


    /** @return - list of cells from buffer  */
    List<Cell> getCells() {
        return _buffer;
    }
}
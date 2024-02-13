package xxl.core;

import java.io.Serial;
import java.io.Serializable;


public abstract class Content implements Serializable {

    @Serial
    private static final long serialVersionUID = 202308132245L;

    /**
    * @return - string of how content was inserted,
    * LiteralInteger: -25, 12
    * LiteralString: 'abc, '
    * Reference: =1;2
    * Function: =ADD(1;2,20), =AVERAGE(1;2:1;4)
    */
    @Override
    public abstract String toString();

    
    /** @return - value of content, which is a Literal */
    abstract Literal value();


    /** @return - value of content as an integer */
    public int asInt() {
        return value().asInt();
    }


    /** @return - value of content as a string */
    public String asString() {
        return value().asString();
    }

}
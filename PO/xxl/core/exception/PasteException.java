package xxl.core.exception;

public class PasteException extends Exception {
    private String _rangeSpecification;

    public PasteException(String rangeSpecification) {
        _rangeSpecification = rangeSpecification;
    }

}
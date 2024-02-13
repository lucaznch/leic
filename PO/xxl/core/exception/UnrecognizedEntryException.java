package xxl.core.exception;

/**
 * Exception for unknown import file entries.
 */
public class UnrecognizedEntryException extends Exception {
  /** Unrecognized entry specification. */
  private String _entrySpecification;
  
  /**
   * @param entrySpecification
   */
  public UnrecognizedEntryException(String entrySpecification) {
    _entrySpecification = entrySpecification;
  }
  
  /**
   * @param entrySpecification
   * @param cause
   */
  public UnrecognizedEntryException(String entrySpecification, Exception cause) {
    super(cause);
    _entrySpecification = entrySpecification;
  }
  
  /**
   * @return the bad entry specification.
   */
  public String getEntrySpecification() {
    return _entrySpecification;
  }
}

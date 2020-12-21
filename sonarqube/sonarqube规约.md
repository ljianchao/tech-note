# sonarqube规约

## "Random" objects should be reused



## Generic exceptions should never be thrown

Using such generic exceptions as Error, RuntimeException, Throwable, and Exception prevents calling methods from handling true, system-generated exceptions differently than application-generated errors.

## Encryption algorithms should be used with secure mode and padding scheme

## Catches should be combined

Since Java 7 it has been possible to catch multiple exceptions at once. Therefore, when multiple catch blocks have the same code, they should be combined for better readability.

Note that this rule is automatically disabled when the project's sonar.java.source is lower than 7.

## Printf-style format strings should be used correctly

Because printf-style format strings are interpreted at runtime, rather than validated by the compiler, they can contain errors that result in the wrong strings being created. This rule statically validates the correlation of printf-style format strings to their arguments when calling the format(...) methods of java.util.Formatter, java.lang.String, java.io.PrintStream, MessageFormat, and java.io.PrintWriter classes and the printf(...) methods of java.io.PrintStream or java.io.PrintWriter classes.

## Parsing should be used to convert "Strings" to primitives

Rather than creating a boxed primitive from a String to extract the primitive value, use the relevant parse method instead. It will be clearer and more efficient.

## "StandardCharsets" constants should be preferred


## Cognitive Complexity of methods should not be too high

## Instance methods should not write to "static" fields

## bugs（可靠性）

### Double-checked locking should not be used

### Resources should be closed

Connections, streams, files, and other classes that implement the Closeable interface or its super-interface, AutoCloseable, needs to be closed after use. Further, that close call must be made in a finally block otherwise an exception could keep the call from being made. Preferably, when class implements AutoCloseable, resource should be created using "try-with-resources" pattern and will be closed automatically.
Failure to properly close resources will result in a resource leak which could bring first the application and then perhaps the box the application is on to their knees.

### "BigDecimal(double)" should not be used

Because of floating point imprecision, you're unlikely to get the value you expect from the BigDecimal(double) constructor.

From the JavaDocs:

> The results of this constructor can be somewhat unpredictable. One might assume that writing new BigDecimal(0.1) in Java creates a BigDecimal which is exactly equal to 0.1 (an unscaled value of 1, with a scale of 1), but it is actually equal to 0.1000000000000000055511151231257827021181583404541015625. This is because 0.1 cannot be represented exactly as a double (or, for that matter, as a binary fraction of any finite length). Thus, the value that is being passed in to the constructor is not exactly equal to 0.1, appearances notwithstanding.

Instead, you should use BigDecimal.valueOf, which uses a string under the covers to eliminate floating point rounding errors, or the constructor that takes a String argument.

### Null pointers should not be dereferenced

A reference to null should never be dereferenced/accessed. Doing so will cause a NullPointerException to be thrown. At best, such an exception will cause abrupt program termination. At worst, it could expose debugging information that would be useful to an attacker, or it could allow an attacker to bypass security measures.

Note that when they are present, this rule takes advantage of @CheckForNull and @Nonnull annotations defined in JSR-305 to understand which values are and are not nullable except when @Nonnull is used on the parameter to equals, which by contract should always work with null.

Noncompliant Code Example

### Conditionally executed code should be reachable

Conditional expressions which are always true or false can lead to dead code. Such code is always buggy and should never be used in production.

### "compareTo" results should not be checked for specific values

While most compareTo methods return -1, 0, or 1, some do not, and testing the result of a compareTo against a specific value other than 0 could result in false negatives.

### Double Brace Initialization should not be used

Because Double Brace Initialization (DBI) creates an anonymous class with a reference to the instance of the owning object, its use can lead to memory leaks if the anonymous inner class is returned and held by other objects. Even when there's no leak, DBI is so obscure that it's bound to confuse most maintainers.

For collections, use Arrays.asList instead, or explicitly add each item directly to the collection.

### Boxing and unboxing should not be immediately reversed

Boxing is the process of putting a primitive value into an analogous object, such as creating an Integer to hold an int value. Unboxing is the process of retrieving the primitive value from such an object.

Since the original value is unchanged during boxing and unboxing, there's no point in doing either when not needed. This also applies to autoboxing and auto-unboxing (when Java implicitly handles the primitive/object transition for you).

## 漏洞（安全性）

## 异味（可维护性）

### Sections of code should not be commented out

Programmers should not comment out code as it bloats programs and reduces readability.
Unused code should be deleted and can be retrieved from source control history if required.

### Empty arrays and collections should be returned instead of null

Returning null instead of an actual array or collection forces callers of the method to explicitly test for nullity, making them more complex and less readable.
Moreover, in many cases, null is used as a synonym for empty.

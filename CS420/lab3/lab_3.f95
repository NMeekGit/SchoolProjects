FUNCTION READ_MATRIX(filename) RESULT(matrix)
    IMPLICIT NONE
    CHARACTER(*), intent(in) :: filename
    INTEGER :: ios
    INTEGER, DIMENSION(2) :: matrix_size
    REAL :: matrix(100,100)

    ! Open file
    OPEN(UNIT=9, FILE=filename, ACTION="READ", STATUS="OLD", IOSTAT=ios)

    ! Check if file properly opened
    IF(ios /= 0) THEN
        PRINT *, "Could not open file ", TRIM(filename), " Error code: ", ios
        STOP
    ENDIF

    ! Grab matrix size
    READ (9, *) matrix_size

    ! Print matrix size
    PRINT *
    WRITE(*, FMT=300) matrix_size(1), matrix_size(2)

    matrix(1,1) = 1.00

    
300 FORMAT ("Matrix: ", i1, "x", i1)
END FUNCTION READ_MATRIX

PROGRAM LAB_3

    IMPLICIT NONE

    CHARACTER(20) :: filename
    REAL :: matrix1(100,100)

    ! Grab file name from user
    PRINT *, "Enter Filename"
    READ *, filename

    matrix1 = READ_MATRIX(filename)

    
100 FORMAT (I6)
200 FORMAT (A)

END PROGRAM LAB_3


PROGRAM LAB_3

    IMPLICIT NONE

    CHARACTER(20) :: filename
    REAL :: matrix1(100,100), matrix2(100,100), matrix_final(100,100)
    INTEGER :: idx, jdx, ios

    ! Grab first file name from user
    PRINT *, "Enter Filename for first matrix"
    READ *, filename

    matrix1 = READ_MATRIX(filename)

    ! Grab second file name from user
    PRINT *, "Enter Filename for second matrix"
    READ *, filename

    matrix2 = READ_MATRIX(filename)

    ! Check matrix sizes are correct
    IF (size(matrix1,2) /= size(matrix2,1)) THEN
        PRINT *, "Matrix 1 Column Size does not match Matrix 2 Row Size"
        STOP
    ENDIF

    ! Multiply the two matrices
    matrix_final = MULT_MATRIX(matrix1, matrix2)

    ! Open output file
    OPEN(UNIT=10, FILE="lab3_output.txt", ACTION="WRITE", STATUS="NEW", IOSTAT=ios)

    ! Check output file opened successful
    IF (ios /= 0) THEN
        PRINT *, "Could not open output file"
        STOP
    ENDIF

    ! Print final matrix
    DO idx = 1, size(matrix_final, 1)
        DO jdx = 1, size(matrix_final, 2)
            write(10, 100, ADVANCE="NO") matrix_final(idx, jdx) 
        END DO
    END DO

    PRINT *, "Multiplication Completed"

100 FORMAT (f20.2)

    CONTAINS

        FUNCTION READ_MATRIX(filename) RESULT(matrix)
            IMPLICIT NONE
            CHARACTER(*), intent(in) :: filename
            INTEGER :: ios, row, col
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

            DO row = 1, matrix_size(1)
                DO col = 1, matrix_size(2)
                    READ(9, *) matrix(row, col)
                END DO
            END DO

            CLOSE(9)

            300 FORMAT ("Matrix: ", i1, "x", i1)
            
        END FUNCTION READ_MATRIX

        FUNCTION MULT_MATRIX(matrix_a, matrix_b) RESULT(matrix_final)
            IMPLICIT NONE
            REAL, INTENT(IN) :: matrix_a(100,100), matrix_b(100,100)
            REAL :: matrix_final(100,100)
            INTEGER idx, jdx, kdx
            INTEGER m, n, p

            m = size(matrix1, 1)
            n = size(matrix1, 2)
            p = size(matrix2, 2)
    
            DO idx = 1, m
                DO jdx = 1, p
                    DO kdx = 1, n
                        matrix_final(idx, jdx) = matrix_a(idx, kdx) * matrix_b(kdx, jdx)
                    END DO
                END DO
            END DO

        END FUNCTION MULT_MATRIX
END PROGRAM LAB_3

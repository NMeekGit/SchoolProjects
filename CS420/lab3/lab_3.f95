! PROGRAM LAB_3.F95
!
! Noah Meeker
! Assignment 3

PROGRAM LAB_3

    IMPLICIT NONE

    CHARACTER(20) :: filename
    REAL, ALLOCATABLE :: matrix1(:,:), matrix2(:,:), matrix_final(:,:)
    INTEGER, DIMENSION(2) :: size_arr
    INTEGER :: idx, jdx, ios, count

    ! Grab first file name from user
    PRINT *, "Enter Filename for first matrix"
    READ *, filename

    matrix1 = READ_MATRIX(filename)

    ! grab matrix size
    PRINT *
    WRITE(*, FMT=300) size(matrix1,1), size(matrix1,2)

    ! Grab second file name from user
    PRINT *, "Enter Filename for second matrix"
    READ *, filename

    matrix2 = READ_MATRIX(filename)
    PRINT *
    WRITE(*, FMT=300) size(matrix2,1), size(matrix2,2)

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
        write(10,*)
    END DO

    PRINT *, "Multiplication Completed"

100 FORMAT (f20.2)
300 FORMAT ("Matrix: ", i1, "x", i1)

    CONTAINS

        FUNCTION READ_MATRIX(filename) RESULT(matrix)
            IMPLICIT NONE
            CHARACTER(*), intent(in) :: filename
            INTEGER :: ios, row, col
            INTEGER, DIMENSION(2) :: matrix_size
            REAL, ALLOCATABLE :: matrix(:,:)

            ! Open file
            OPEN(UNIT=9, FILE=filename, ACTION="READ", STATUS="OLD", IOSTAT=ios)

            ! Check if file properly opened
            IF(ios /= 0) THEN
                PRINT *, "Could not open file ", TRIM(filename), " Error code: ", ios
                STOP
            ENDIF

            ! Grab matrix size
            ! 
            ! matrix_size(1) = row
            ! matrix_size(2) = col
            READ (9, *) matrix_size

            ALLOCATE(matrix(matrix_size(1),matrix_size(2)))

            ! Read in matrix from file
            DO row = 1, matrix_size(1)
                READ(9, *) (matrix(row, col), col = 1, matrix_size(2))
            END DO

            CLOSE(9)
            
        END FUNCTION READ_MATRIX

        ! Function to mulitply two matrices
        FUNCTION MULT_MATRIX(matrix_a, matrix_b) RESULT(matrix_final)
            IMPLICIT NONE
            REAL, INTENT(IN) :: matrix_a(:,:), matrix_b(:,:)
            REAL, ALLOCATABLE :: matrix_final(:,:)
            INTEGER :: idx, jdx, kdx
            INTEGER :: m, n, p

            ! m = matrix1 row size
            ! n = matrix1 col size
            ! p = matrix2 row size
            m = size(matrix1, 1)
            n = size(matrix1, 2)
            p = size(matrix2, 2)

            ! Allocate final matrix size
            ALLOCATE(matrix_final(m,p))
    
            ! Multiply
            DO idx = 1, m
                DO jdx = 1, p
                    DO kdx = 1, n
                        matrix_final(idx, jdx) = matrix_a(idx, kdx) * matrix_b(kdx, jdx)
                    END DO
                END DO
            END DO

        END FUNCTION MULT_MATRIX
END PROGRAM LAB_3

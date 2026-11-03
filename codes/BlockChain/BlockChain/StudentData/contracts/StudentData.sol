// SPDX-License-Identifier: MIT
pragma solidity ^0.8.28;

/**
 * @title StudentData
 * @dev Smart contract demonstrating Structures, Arrays, and Fallback functions,
 *      deployed on Ethereum with gas and transaction fee tracking.
 */
contract StudentData {
    // 1. Structure definition for Student
    struct Student {
        uint256 id;
        string name;
        uint8 age;
        string course;
    }

    // 2. Dynamic Array to store student records
    Student[] public students;

    // Events for logging actions on-chain
    event StudentAdded(uint256 indexed id, string name, uint8 age, string course);
    event FallbackTriggered(address indexed sender, uint256 value, bytes data);
    event ReceivedEther(address indexed sender, uint256 value);

    /**
     * @notice Add a new student record using Struct and Array
     * @param _id Student unique ID
     * @param _name Student full name
     * @param _age Student age
     * @param _course Student course or department
     */
    function addStudent(
        uint256 _id,
        string memory _name,
        uint8 _age,
        string memory _course
    ) public {
        require(_id > 0, "Student ID must be greater than 0");
        require(bytes(_name).length > 0, "Student name cannot be empty");

        // Construct student using struct
        Student memory newStudent = Student({
            id: _id,
            name: _name,
            age: _age,
            course: _course
        });

        // Store into dynamic array
        students.push(newStudent);

        emit StudentAdded(_id, _name, _age, _course);
    }

    /**
     * @notice Retrieve the total number of students
     */
    function getStudentCount() public view returns (uint256) {
        return students.length;
    }

    /**
     * @notice Retrieve student details by array index
     * @param index The array index of the student
     */
    function getStudent(uint256 index)
        public
        view
        returns (
            uint256 id,
            string memory name,
            uint8 age,
            string memory course
        )
    {
        require(index < students.length, "Index out of bounds");
        Student storage s = students[index];
        return (s.id, s.name, s.age, s.course);
    }

    /**
     * @notice Retrieve all students in the array
     */
    function getAllStudents() public view returns (Student[] memory) {
        return students;
    }

    /**
     * @notice 3. Fallback function
     * Executed when a non-existent function signature is invoked or arbitrary data is sent.
     */
    fallback() external payable {
        emit FallbackTriggered(msg.sender, msg.value, msg.data);
    }

    /**
     * @notice Receive function
     * Executed when plain ether is sent directly to the contract without data.
     */
    receive() external payable {
        emit ReceivedEther(msg.sender, msg.value);
    }
}

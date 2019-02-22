# hybrid_genetic_algorithm_JSP
<p>
This project implements a hybrid genetic algorithm for the job scheduling problem
<a href="http://mauricio.resende.info/doc/hgajss.pdf">link to paper</a>.<br/>
The project was developed under GNU/Linux 64-bit OS.
<p>
<h5> Requirements </h5>
<ul>
    <li>CMake >= 3.0</li>
    <li>make - a GNU compiler for C language</li>
    <li>C++ compiler supporting C++11 standard</li>
    <li>
    	JSON for Modern C++  3.2 or newer
    	<ul>
    		Short guide how to install this library on linux machine:
    		<li>Clone repository:<br/>
    				git clone https://github.com/nlohmann/json</li>
    		<li>Change your working directory:<br/>
    		 		cd json</li>
    		 <li>Build the library:<br/>
    		 mkdir build; cd build; cmake ../; make install</li>
    		 <li>Say: "Thanks for this foolproof guide!"</li>
    	</ul>
    </li>
</ul>
</p>
<h5>How to build the project:</h5>
<ul>
    <li>mkdir build</li>
    <li>cd build</li>
    <li>cmake ../</li>
    <li>make</li>
</ul>
<p>Run ./hybrid_algorithm to start the algorithm working on default dataset.<br/>
To change the working dataset - choose the desired dataset index passed<br/>
as a parameter in the run_test_case function, called from main.</p>

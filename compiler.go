package main

import (
	"fmt"		
	"os/exec"
	"time"
	"bufio"
	"os"
	"bytes"
	"regexp"	
	"path/filepath"
)

func main() {

	compile:

	fmt.Println("\n\x1b[32m× Compiling...", "\x1b[0m")
	start := time.Now()	

	PATH_COMPILER := "E:/SAMP dev/compiler/bin/pawncc.exe"

	//cmd := exec.Command(PATH_COMPILER, "../gamemodes/main.pwn", "-Dgamemodes", "-;+", "-(+", "-d3", "-Z+")
	/*
		"cmd": [
			"pawncc.exe",
			"$file", 
	        "-o$file_path/$file_base_name", 
	        "-;+", 
	        "-(+", 
	        "-d3"
		],
	*/
	//pawncc "E:/SAMP dev/samp-smokers/gamemodes/main.pwn" "-DE:/SAMP dev/samp-smokers/gamemodes" "-;+" "-(+" "-d3"
	cmd := exec.Command(PATH_COMPILER, "E:/SAMP dev/samp-smokers/gamemodes/main.pwn", "-DE:/SAMP dev/samp-smokers/gamemodes", "-;+", "-(+", "-d3")
	var out bytes.Buffer
	var stderr bytes.Buffer
	cmd.Stdout = &out
	cmd.Stderr = &stderr
	cmd.Run()	
//	var myReg = regexp.MustCompile(`(?m)(?P<File>\w+\.(?P<FileType>pwn|inc|p))\((?P<Line>[^)]+\))\s\:\s(?P<Error>error|warning|fatal error)\s\w+\:(?P<Reason>[^\n]+)`)
 	var myReg = regexp.MustCompile(`(?P<Path>[\w\/]+)\/(?P<File>\w+\.(?P<FileType>pwn|inc|p))\((?P<Line>[^)]+\))\s\:\s(?P<Error>error|warning|fatal error)\s\w+\:(?P<Reason>[^\n]+)`)

 	for z, match := range myReg.FindAllString(stderr.String(), -1) {
       // fmt.Println(match, "found at index", z)
        //fmt.Println("this shit? >", match)

        if z != -1 {
	        res := myReg.FindStringSubmatch(match)
		  //  names := myReg.SubexpNames()		  	
		   // for i, _ := range res {
		     //   if i != 0 {
        	   //    	fmt.Println(res[i])

		         	//fmt.Println("\x1b[35m", "Module: ", "\x1b[0m", res[0])	
		         	var path = res[1]	      
		         	var filename = res[2]
		         	var filetype = returnFileType(res[3])
		            var line = res[4]   	        	
		            var error = returnErrorType(res[5])
		            var reason = res[6]
		            fmt.Println("\x1b[35m", filetype, "\x1b[0m", " ", "File: \x1b[90m(", path, "/", filename, ") \x1b[0m\n\t", error, "\x1b[90mLine: (", line, ": ", reason, " \x1b[0m\n")    
		           //	break; 	
		      // }

		    //}
		} 
		
    }
    TotalFatalErrors := 0
	TotalErrors := 0
	TotalWarnings := 0

    re := regexp.MustCompile(`fatal error`)
	results :=  re.FindAll([]byte(stderr.String()), -1)
	TotalFatalErrors = len(results)

	re = regexp.MustCompile(`error`)
	results =  re.FindAll([]byte(stderr.String()), -1)
	TotalErrors = len(results)

	re = regexp.MustCompile(`warning`)
	results =  re.FindAll([]byte(stderr.String()), -1)
	TotalWarnings = len(results)
/*
\\(\w+\.(pwn|inc|p))\(([^)]+\))\s\:\s(error|warning|fatal error)\s\w+\:([^\n]+)
*/



	
	//----------------------------------------------------------------------	

	t := time.Now()
	elapsed := t.Sub(start)

	fmt.Println("\n----------------------------------------------------")

	

	printStatus(TotalFatalErrors, 	"Fatal error(s)")
	printStatus(TotalErrors, 		"Error(s)")
	printStatus(TotalWarnings, 		"Warning(s)")

	//Reset
	TotalFatalErrors = 0
	TotalErrors = 0
	TotalWarnings = 0

	fmt.Println("\n\x1b[32m× Compilling took:\x1b[0m \x1b[33m", elapsed, "\x1b[0m")

	//Get numbers of lines and number of listing files
	exts := []string{".pwn", ".p"}
	//Listing
	FoundedFiles := 0;
	var found_files []string
	err := filepath.Walk("E:/SAMP dev/samp-smokers/gamemodes", func(path string, info os.FileInfo, err error) error {
		//Checking files
		if stringInSlice(filepath.Ext(path), exts) {
			found_files = append(found_files, path)
			FoundedFiles++
		} 
		return nil
	})
	if err != nil {
		panic(err)
	}
	TotalLines := 0;
	for _, file := range found_files {		
		lines := LinesInFile(file)
		TotalLines += len(lines)
	}
	fmt.Println("\n----------------------------------------------------\n")
	printStatus(TotalLines, "Total Line(s)")
	printStatus(FoundedFiles, "Total File(s)")
	fmt.Println("\n \n Press enter key to compile again")
	bufio.NewReader(os.Stdin).ReadBytes('\n')
	goto compile
}

func stringInSlice(v string, ss []string) bool {
    for _, s := range ss {
        if s == v {
            return true
        }
    }
    return false
}

func LinesInFile(fileName string) []string {
    f, _ := os.Open(fileName)
    // Create new Scanner.
    scanner := bufio.NewScanner(f)
    result := []string{}
    // Use Scan.
    for scanner.Scan() {
        line := scanner.Text()
        // Append line to result.
        result = append(result, line)
    }
    return result
}

func returnErrorType(str string) (formstr string) {
	switch str {
		case "warning":
		{
			formstr = "\x1b[93mWarning\x1b[0m"
		}
		case "error":
		{
			formstr = "\x1b[31mError\x1b[0m"
		}
		case "fatal error":
		{
			formstr = "\x1b[101mFatal error\x1b[0m"
		}
		default:
		{
			formstr = "\x1b[31mUnknown\x1b[0m"
		}
	}	
	return formstr
}
func returnFileType(str string) (formstr string) {
	
	switch str {
		case "pwn":
		{
			formstr = "\x1b[35mModule\x1b[0m"
		}
		case "p":
		{
			formstr = "\x1b[35mModule\x1b[0m"
		}
		case "inc":
		{
			formstr = "\x1b[36mInclude\x1b[0m"
		}
		default:
		{
			formstr = "\x1b[31mUnknown\x1b[0m"
		}
	}	
	return formstr
}
func printStatus (num int, str string) {
	if num > 0	{
		fmt.Println("\x1b[31m ", num, "\x1b[0m", str)
	} else {
		fmt.Println("\x1b[32m ", num, "\x1b[0m", str)	
	}	
}

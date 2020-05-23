package main
 
import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"io/ioutil"
	"strconv"
)

type UpdateInfo struct{
	Filename string
	Version float64
}

type InfoSlice struct{
	Updatefiles []UpdateInfo
}

func parseJson(path string) (InfoSlice, error) {
	var info InfoSlice
	var content []byte
	f, err := os.Open(path)
	defer f.Close()
	if err != nil {
		return info, err
	}
	content, err = ioutil.ReadAll(f)
	if err != nil {
		fmt.Println("open file error: "+ err.Error())
		return info,err
	}
	err = json.Unmarshal([]byte(content), &info)
	if err != nil {
		fmt.Println("ERROR: ", err.Error())
		return info,err
	}
	return info,nil
}

func downloadFile(url string,filename string)(){
	var (
		fsize   int64
		buf     = make([]byte, 32*1024)
		written int64
	)
	res, err := http.Get(url)
    if err != nil {
        panic(err)
    }
    file, err := os.Create("temp/"+filename)
    if err != nil {
        panic(err)
	}
	fsize, err = strconv.ParseInt(res.Header.Get("Content-Length"), 10, 32)
	if err != nil {
		panic(err)
	}
	//io.Copy(file, res.Body)
	defer file.Close()
	if res.Body == nil {
		panic(err)
	}
	defer res.Body.Close()
	//下面是 io.copyBuffer() 的简化版本
	for {
	//读取bytes
		nr, er := res.Body.Read(buf)
		if nr > 0 {
		//写入bytes
		nw, ew := file.Write(buf[0:nr])
		//数据长度大于0
		if nw > 0 {
			written += int64(nw)
		}
			//写入出错
		if ew != nil {
			err = ew
			break
		}
		//读取是数据长度不等于写入的数据长度
		if nr != nw {
			err = io.ErrShortWrite
			break
		}
		}
		if er != nil {
			if er != io.EOF {
				err = er
			}
			break
		}
		//没有错误了快使用 callback
		fmt.Printf("下载进度：%.2f/%.2f MB\r",float64(written)/(1024*1024),float64(fsize)/(1024*1024))
		//fmt.Println(fsize, written)
	}
}

func main() {
	info,err := parseJson("./version.json")
	var currJsonVersion float64
	if err == nil{
		for _,val := range info.Updatefiles{
			if val.Filename == "version.json"{
				currJsonVersion = val.Version
			}
		}
	}else{
		fmt.Println("nil")
	}
	fmt.Println("检查更新中，请稍后……")
	downloadFile("https://github.com/liupuchun/oldchatroom/releases/download/1.0/version.json","version.json")
	newinfo,err := parseJson("./temp/version.json")
	var newJsonVersion float64
	if err == nil{
		for _,val := range newinfo.Updatefiles{
			if val.Filename == "version.json"{
				newJsonVersion = val.Version
				break
			}
		}
	}else{
		fmt.Println("nil")
	}
	if newJsonVersion > currJsonVersion{
		fmt.Println("有新版本！是否更新？[yes/no]")
		var ok string
		fmt.Scan(&ok)
		if ok == "yes"{
			downloadFile("http://39.97.106.216:10000/f/a5123dda5d2b4d079944/?dl=1","ChatClient.exe")
			fmt.Println("下载成功，开始更新……");
			os.Remove("ChatClient.exe")
			os.Remove("version.json")
			oldPath, newPath := "./temp/ChatClient.exe", "ChatClient.exe"
			os.Rename(oldPath, newPath)
			oldPath, newPath = "./temp/version.json", "version.json"
			os.Rename(oldPath, newPath)
			os.Remove("./temp/ChatClient.exe")
			os.Remove("./temp/version.json")
			fmt.Println("更新成功！");
			}else{
			fmt.Printf("n%s",ok)
		}
	}else{
		fmt.Println("无需更新！")
		os.Remove("./temp/version.json")
	}
}
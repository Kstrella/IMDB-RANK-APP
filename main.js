

const API_KEY = 'api_key=fe6e90e8a90b5dba2e224b69ae1dbfef';
const BASE_URL = 'https://api.themoviedb.org/3';
const API_URL = BASE_URL + '/discover/movie?sort_by=popularity.desc&'+API_KEY;
const IMG_URL = 'https://image.tmdb.org/t/p/w500';
const searchURL = BASE_URL + '/search/movie?'+API_KEY;

const main = document.getElementById('main');
const form =  document.getElementById('form');
const search = document.getElementById('search');
const tagEl = document.getElementById('tags');



//the following is for the genres
const genres = [
    {
      "id": 28,
      "name": "Action"
    },
    {
      "id": 12,
      "name": "Adventure"
    },
    {
      "id": 16,
      "name": "Animation"
    },
    {
      "id": 35,
      "name": "Comedy"
    },
    {
      "id": 80,
      "name": "Crime"
    },
    {
      "id": 99,
      "name": "Documentary"
    },
    {
      "id": 18,
      "name": "Drama"
    },
    {
      "id": 10751,
      "name": "Family"
    },
    {
      "id": 14,
      "name": "Fantasy"
    },
    {
      "id": 36,
      "name": "History"
    },
    {
      "id": 27,
      "name": "Horror"
    },
    {
      "id": 10402,
      "name": "Music"
    },
    {
      "id": 9648,
      "name": "Mystery"
    },
    {
      "id": 10749,
      "name": "Romance"
    },
    {
      "id": 878,
      "name": "Science Fiction"
    },
    {
      "id": 10770,
      "name": "TV Movie"
    },
    {
      "id": 53,
      "name": "Thriller"
    },
    {
      "id": 10752,
      "name": "War"
    },
    {
      "id": 37,
      "name": "Western"
    }
  ]


  var selectedGenre = [];
  setGenre();

  function setGenre(){
    //sets tag based on input dynamically
    tagEl.innerHTML = '';
    genres.forEach(genre =>{
        const t = document.createElement('div');
        t.classList.add('tag')
        t.id = genre.id;
        t.innerText = genre.name;
        t.addEventListener('click', () => {
            //if we have an empty arry push the id
            if(selectedGenre.length == 0){
                selectedGenre.push(genre.id);
            } else {
                if(selectedGenre.includes(genre.id)){
                    //If the genre has already been selected remove
                    selectedGenre.forEach((id,idx) => {
                        if(id == genre.id){
                            selectedGenre.splice(idx,1);
                        }
                    })
                } else {
                    //if id doesnt exist we add it to array
                    selectedGenre.push(genre.id);
                }
            }
            console.log(selectedGenre)
            //this part gets the genre from API  
            getMovies(API_URL + '&with_genres='+encodeURI(selectedGenre.join(',')))
            higlightSelection();
        })
        //get genre names to display
        tagEl.append(t);
    
    })
  }



//this function is used to higlight selected optiopns on genre and year
function higlightSelection() {
    const tags = document.querySelectorAll('.tag');
    tags.forEach(tag => {
        tag.classList.remove('highlight');
    })
   
    if(selectedGenre.length != 0){
    selectedGenre.forEach(id => {
        const higlightedTag = document.getElementById(id);
        higlightedTag.classList.add('highlight');
    })
}
}

 //this function cleares selected generers


getMovies(API_URL);
function getMovies(url) {

    fetch(url).then(res => res.json()).then(data => {
        console.log(data.results)
        if(data.results.length !== 0){
            showMovies(data.results);
        }else{
            main.innerHTML= `<h1 class="no-results">No Results Found</h1>`
        }
        
    })

}

//injecting API data into HTML elements
function showMovies(data) {
    main.innerHTML = '';

    data.forEach(movie => {
        const {title, poster_path, vote_average, overview} = movie;
        const movieEl = document.createElement('div');
        movieEl.classList.add('movie');
        movieEl.innerHTML = `
             <img src="${poster_path? IMG_URL+poster_path: "https://eagle-sensors.com/wp-content/uploads/unavailable-image.jpg"}" alt="${title}">
            <div class="movie-info">
                <h3>${title}</h3>
                <span class="${getColor(vote_average)}">${vote_average}</span>
            </div>
            <div class="overview">
                <h3>Description</h3>
                ${overview}
            </div>
        
        `

        main.appendChild(movieEl);
    })
}

//rating color change
function getColor(vote) {
    if(vote>= 8){
        return 'green'
    }else if(vote >= 5){
        return "orange"
    }else{
        return 'red'
    }
}

form.addEventListener('submit', (e) => {
    e.preventDefault();

    const searchTerm = search.value;

    if(searchTerm) {
        getMovies(searchURL+'&query='+searchTerm)
    }else{
        getMovies(API_URL);
    }

})
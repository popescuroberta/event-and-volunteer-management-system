// Data storage
let events = JSON.parse(localStorage.getItem('events')) || [];
let volunteers = JSON.parse(localStorage.getItem('volunteers')) || [];
let tasks = JSON.parse(localStorage.getItem('tasks')) || [];

// Toggle form visibility
function toggleForm(formId) {
  const forms = document.querySelectorAll('.form-section');
  forms.forEach(form => form.style.display = 'none');
  document.getElementById(formId).style.display = 'block';
}

// Handle event form submission
document.getElementById("createEventButton").addEventListener("click", function(event) {
  event.preventDefault();

  const eventName = document.getElementById("eventName").value;
  const eventDate = document.getElementById("eventDate").value;
  const eventDescription = document.getElementById("eventDescription").value;

  if (!eventName || !eventDate || !eventDescription) {
    alert("Please fill in all the fields!");
    return;
  }

  const newEvent = { name: eventName, date: eventDate, description: eventDescription };
  events.push(newEvent);
  localStorage.setItem('events', JSON.stringify(events));
  updateEventList();
});

// Handle volunteer form submission
document.getElementById("addVolunteerButton").addEventListener("click", function(event) {
  event.preventDefault();

  const volunteerName = document.getElementById("volunteerName").value;
  const volunteerEmail = document.getElementById("volunteerEmail").value;
  const volunteerPhone = document.getElementById("volunteerPhone").value;

  if (!volunteerName || !volunteerEmail || !volunteerPhone) {
    alert("Please fill in all the fields!");
    return;
  }

  const newVolunteer = { name: volunteerName, email: volunteerEmail, phone: volunteerPhone };
  volunteers.push(newVolunteer);
  localStorage.setItem('volunteers', JSON.stringify(volunteers));
  updateVolunteerList();
});

// Functions to update lists
function updateEventList() {
  const eventList = document.getElementById("eventList");
  eventList.innerHTML = "";
  events.forEach(event => {
    const li = document.createElement("li");
    li.textContent = `${event.name} (Date: ${event.date}) - ${event.description}`;
    eventList.appendChild(li);
  });
}

// Update lists on load
updateEventList();
